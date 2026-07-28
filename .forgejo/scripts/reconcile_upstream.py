"""Reconcile forge state/ labels against upstream pull request reality.

The state/ labels are the authority on where each fix stands, but nothing updates them
when an upstream pull request merges, so state/submitted goes stale silently. This
script closes that gap by detection, not mutation: it never edits a label, it posts one
comment on the affected forge item saying what changed upstream and which label now
looks wrong. Deciding remains a human's job; noticing no longer is.

Matching is by branch name: a forge pull request whose head branch equals the head ref
of one of our upstream pull requests. A forge issue has no branch of its own, so it
borrows the one from the pull request that closes it, which the forge records as a
pull_ref event on the issue. Anything still unmatched after that is listed in the run
log for eyeballing rather than commented on.

Idempotency: before commenting, the script scans the item's existing comments for the
marker line this script writes. Same upstream state seen before means no new comment.

Environment:
  FORGE_TOKEN     Forgejo API token (Actions provides one)
  FORGE_API       e.g. https://git.snalpha.com/api/v1
  FORGE_REPO      e.g. alex/jak-project
  UPSTREAM        e.g. open-goal/jak-project
  AUTHOR          upstream PR author to track, e.g. semeaj
  UPSTREAM_TOKEN  optional GitHub token, public reads only. Without one the GitHub
                  calls are anonymous, and the anonymous budget is 60 requests an hour
                  shared by every client behind the runner's egress IP. That budget
                  belongs to the whole homelab, not to this job, which is how the
                  2026-07-27 run died on HTTP 403 while making its first request.
"""
import json
import os
import sys
import time
import urllib.error
import urllib.request

FORGE_TOKEN = os.environ.get("FORGE_TOKEN")
FORGE_API = os.environ.get("FORGE_API", "https://git.snalpha.com/api/v1")
FORGE_REPO = os.environ.get("FORGE_REPO", "alex/jak-project")
UPSTREAM = os.environ.get("UPSTREAM", "open-goal/jak-project")
AUTHOR = os.environ.get("AUTHOR", "semeaj")
UPSTREAM_TOKEN = os.environ.get("UPSTREAM_TOKEN") or None

MARKER = "reconcile-bot:"

# How long we are willing to sit out a rate-limit window inside the job's timeout.
# The anonymous core window is an hour wide, so waiting it out is not an option; the
# search window is about a minute, which is worth waiting for.
MAX_RATE_LIMIT_WAIT_S = 300


class ReconcileError(RuntimeError):
    """A condition the run cannot recover from and a human has to act on."""


class RateLimited(ReconcileError):
    """GitHub refused a request because the request budget is spent."""


def http(url, token=None, method="GET", body=None, auth_scheme="token"):
    headers = {"Content-Type": "application/json",
               "User-Agent": "jak-project-reconcile-bot"}
    if token:
        headers["Authorization"] = f"{auth_scheme} {token}"
    req = urllib.request.Request(
        url, method=method, headers=headers,
        data=json.dumps(body).encode() if body is not None else None)
    with urllib.request.urlopen(req) as r:
        return json.loads(r.read() or "{}")


def forge(path, method="GET", body=None):
    return http(f"{FORGE_API}/repos/{FORGE_REPO}{path}", FORGE_TOKEN, method, body)


def rate_limit_wait(headers):
    """Seconds to wait before this 403/429 could succeed, or None if it is not a
    rate limit at all (a 403 is also how GitHub says forbidden)."""
    retry_after = headers.get("retry-after")
    if retry_after:
        try:
            return max(1, int(retry_after))
        except ValueError:
            # The HTTP-date form of Retry-After. Rare from GitHub, and guessing a
            # duration from it is worse than reporting the refusal as-is.
            return None
    if headers.get("x-ratelimit-remaining") != "0":
        return None
    reset = headers.get("x-ratelimit-reset")
    if not reset:
        return None
    try:
        return max(1, int(reset) - int(time.time()) + 1)
    except ValueError:
        return None


def github(path, _retried=False):
    try:
        return http(f"https://api.github.com{path}", UPSTREAM_TOKEN,
                    auth_scheme="Bearer")
    except urllib.error.HTTPError as e:
        if e.code not in (403, 429):
            raise
        wait = rate_limit_wait(e.headers)
        if wait is None:
            raise
        if not _retried and wait <= MAX_RATE_LIMIT_WAIT_S:
            print(f"rate limited on {path}, window reopens in {wait}s; waiting")
            time.sleep(wait)
            return github(path, _retried=True)
        remedy = ("The token in UPSTREAM_TOKEN is itself exhausted, which for 5000 "
                  "requests an hour means something is looping."
                  if UPSTREAM_TOKEN else
                  "These calls are anonymous, so they draw on a 60-per-hour budget "
                  "shared by every client behind this runner's egress IP. Set the "
                  "UPSTREAM_GITHUB_TOKEN repository secret to get a private 5000; a "
                  "GitHub token with no scopes at all is enough for public reads.")
        raise RateLimited(
            f"GitHub refused GET {path} with HTTP {e.code}: request budget spent, "
            f"the window reopens in {wait}s. {remedy}") from e


def github_budget():
    """One line on where the GitHub budget stands. /rate_limit is itself exempt, so
    this reports the situation even when everything else would be refused."""
    resources = github("/rate_limit")["resources"]
    mode = "authenticated" if UPSTREAM_TOKEN else "anonymous"
    return " ".join([mode] + [f"{name} {r['remaining']}/{r['limit']}"
                              for name, r in resources.items()
                              if name in ("core", "search")])


def upstream_prs_by_branch():
    """Map upstream head branch -> (number, state) for every pull request AUTHOR
    opened on UPSTREAM.

    Search rather than a /pulls listing. That listing is newest-first and this job read
    one page of it, so our pull requests dropped out of view the moment a hundred newer
    ones existed upstream, and the reconciler would have gone quietly blind rather than
    red. Searching by author is bounded by our own output instead of upstream's rate of
    change.
    """
    query = f"repo:{UPSTREAM}+author:{AUTHOR}+is:pr"
    found = github(f"/search/issues?q={query}&per_page=100&sort=created&order=asc")
    items = found.get("items", [])
    total = found.get("total_count", 0)
    if found.get("incomplete_results"):
        raise ReconcileError(f"GitHub timed out the search for {query} and returned a "
                             f"partial result; a partial result would read as pull "
                             f"requests that do not exist")
    if total > len(items):
        raise ReconcileError(f"search matched {total} pull requests by {AUTHOR} but "
                             f"returned {len(items)}; this job reads a single page and "
                             f"now needs pagination")

    by_branch = {}
    for item in items:
        num = item["number"]
        state = ("merged" if (item.get("pull_request") or {}).get("merged_at")
                 else item["state"])
        # The search result carries no head ref, and the head ref is the only thing
        # that ties a forge pull request to an upstream one. It survives deletion of
        # the branch, which is what makes a closed staging pull request matchable.
        head = github(f"/repos/{UPSTREAM}/pulls/{num}")["head"]["ref"]
        # Ascending by number above, so a resubmitted branch resolves to its latest
        # pull request rather than to whichever one search happened to rank first.
        by_branch[head] = (num, state)
    return by_branch


def closing_pull_heads(num):
    """Head branches of the forge pull requests that close issue `num`, best first.

    An issue carries no branch, so it cannot be matched to an upstream pull request the
    way a forge pull request can. The forge does hold the link: opening a pull request
    whose body says "Closes #N" writes a pull_ref event onto issue N with ref_action
    'closes'.

    That action degrades to 'neutered' when the closing line is later edited out of the
    body, which happens routinely here because pull request bodies get rewritten to read
    as-if-upstream before submission. A neutered reference is stale rather than false,
    and it is still evidence of which branch carries the fix, so it is ranked below a
    live one instead of being dropped.
    """
    events = forge(f"/issues/{num}/timeline?limit=100")
    refs = {"closes": [], "neutered": []}
    for ev in events:
        if ev.get("type") != "pull_ref":
            continue
        number = (ev.get("ref_issue") or {}).get("number")
        if number and ev.get("ref_action") in refs:
            refs[ev["ref_action"]].append(number)

    heads = []
    for action in ("closes", "neutered"):
        for pr in refs[action]:
            head = (forge(f"/pulls/{pr}").get("head") or {}).get("ref")
            # A pull request whose branch has been deleted can report refs/pull/N/head
            # as its head. That is a forge-internal ref, not a branch anyone could have
            # pushed upstream, and matching on it would be meaningless.
            if head and not head.startswith("refs/"):
                heads.append((pr, action, head))
    return heads


def main():
    if not FORGE_TOKEN:
        raise ReconcileError("FORGE_TOKEN is unset, so the forge half of this run "
                             "cannot authenticate")

    print(f"github budget: {github_budget()}")
    by_branch = upstream_prs_by_branch()
    print(f"upstream PRs by {AUTHOR}: "
          f"{ {b: f'#{n} {s}' for b, (n, s) in by_branch.items()} }")

    deltas = 0
    unmatched = []
    for label in ("state/submitted", "state/staged"):
        items = forge(f"/issues?labels={label}&state=all&type=all&limit=50")
        for it in items:
            num = it["number"]
            via = None
            if it.get("pull_request"):
                # The issues listing returns pull requests as issue objects without a
                # head field; fetch the pull detail for those. The head ref name
                # normally survives in the record even after the branch is deleted,
                # which is what makes a closed staging pull request matchable at all.
                head = ((forge(f"/pulls/{num}").get("head") or {}).get("ref"))
            else:
                head = None
                for pr, action, candidate in closing_pull_heads(num):
                    if candidate in by_branch:
                        head, via = candidate, (pr, action)
                        break
            if not head:
                if label == "state/submitted":
                    unmatched.append((num, "no branch to match it by"))
                continue
            if head not in by_branch:
                if label == "state/submitted":
                    unmatched.append((num, f"branch `{head}` matches no upstream "
                                           f"pull request by {AUTHOR}"))
                continue
            up_num, up_state = by_branch[head]
            # state/submitted expects an open upstream PR; anything else is a delta.
            # state/staged expects NO upstream PR; any hit at all is a delta.
            expected_ok = (label == "state/submitted" and up_state == "open")
            if expected_ok:
                continue
            marker = f"{MARKER} upstream #{up_num} state={up_state}"
            comments = forge(f"/issues/{num}/comments")
            if any(marker in (c.get("body") or "") for c in comments):
                continue
            verdict = {
                "merged": "label should move to state/merged-upstream",
                "closed": "upstream closed without merging; label needs a human call",
                "open": "an upstream PR exists; label should move to state/submitted",
            }[up_state]
            provenance = ""
            if via:
                via_num, via_action = via
                stale = (" a closing reference its body no longer carries, so check the"
                         " match" if via_action == "neutered" else "")
                provenance = (f" This issue has no branch of its own; it was matched "
                              f"through pull request #{via_num}, which closes it"
                              f"{' (' + stale.strip() + ')' if stale else ''}.")
            body = (f"{marker}\n\nUpstream pull request "
                    f"open-goal/jak-project#{up_num} for branch `{head}` is now "
                    f"**{up_state}**, but this item carries `{label}`: {verdict}."
                    f"{provenance} "
                    f"Detected by the scheduled reconciliation run; labels are never "
                    f"changed automatically.")
            forge(f"/issues/{num}/comments", "POST", {"body": body})
            print(f"DELTA: forge #{num} [{label}] vs upstream #{up_num} ({up_state})"
                  + (f" via forge #{via[0]} ({via[1]})" if via else ""))
            deltas += 1

    if unmatched:
        print("state/submitted items this run could not match (check by hand):")
        for num, why in sorted(unmatched):
            print(f"  #{num}: {why}")
    print(f"done: {deltas} delta(s) reported")


if __name__ == "__main__":
    try:
        main()
    except ReconcileError as e:
        # A known condition reads better as a sentence than as a traceback. Anything
        # unknown still gets the traceback, because that is the part worth reading.
        print(f"reconciliation failed: {e}", file=sys.stderr)
        sys.exit(1)

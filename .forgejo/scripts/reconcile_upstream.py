"""Reconcile forge state/ labels against upstream pull request reality.

The state/ labels are the authority on where each fix stands, but nothing updates them
when an upstream pull request merges, so state/submitted goes stale silently. This
script closes that gap by detection, not mutation: it never edits a label, it posts one
comment on the affected forge item saying what changed upstream and which label now
looks wrong. Deciding remains a human's job; noticing no longer is.

Matching is by branch name: a forge pull request whose head branch equals the head ref
of one of our upstream pull requests. Forge issues cannot be matched this way, so items
that carry state/submitted but match no upstream head are listed in the run log for
eyeballing rather than commented on.

Idempotency: before commenting, the script scans the item's existing comments for the
marker line this script writes. Same upstream state seen before means no new comment.

Environment:
  FORGE_TOKEN  Forgejo API token (Actions provides one)
  FORGE_API    e.g. https://git.snalpha.com/api/v1
  FORGE_REPO   e.g. alex/jak-project
  UPSTREAM     e.g. open-goal/jak-project
  AUTHOR       upstream PR author to track, e.g. semeaj
"""
import json
import os
import sys
import urllib.request

FORGE_TOKEN = os.environ["FORGE_TOKEN"]
FORGE_API = os.environ.get("FORGE_API", "https://git.snalpha.com/api/v1")
FORGE_REPO = os.environ.get("FORGE_REPO", "alex/jak-project")
UPSTREAM = os.environ.get("UPSTREAM", "open-goal/jak-project")
AUTHOR = os.environ.get("AUTHOR", "semeaj")

MARKER = "reconcile-bot:"


def http(url, token=None, method="GET", body=None):
    headers = {"Content-Type": "application/json"}
    if token:
        headers["Authorization"] = "token " + token
    req = urllib.request.Request(
        url, method=method, headers=headers,
        data=json.dumps(body).encode() if body is not None else None)
    with urllib.request.urlopen(req) as r:
        return json.loads(r.read() or "{}")


def forge(path, method="GET", body=None):
    return http(f"{FORGE_API}/repos/{FORGE_REPO}{path}", FORGE_TOKEN, method, body)


def github(path):
    # Unauthenticated: public data, one page a day is far inside the rate limit.
    return http(f"https://api.github.com{path}")


def main():
    upstream_prs = [p for p in github(f"/repos/{UPSTREAM}/pulls?state=all&per_page=100")
                    if p["user"]["login"] == AUTHOR]
    by_branch = {}
    for p in upstream_prs:
        state = "merged" if p.get("merged_at") else p["state"]
        by_branch[p["head"]["ref"]] = (p["number"], state)
    print(f"upstream PRs by {AUTHOR}: "
          f"{ {b: s for b, (n, s) in by_branch.items()} }")

    deltas = 0
    unmatched = []
    for label in ("state/submitted", "state/staged"):
        items = forge(f"/issues?labels={label}&state=all&type=all&limit=50")
        for it in items:
            num = it["number"]
            # The issues listing returns pull requests as issue objects without a head
            # field; fetch the pull detail for those. The head ref name survives in the
            # PR record even after its branch is deleted, which is what makes closed
            # staging PRs matchable at all.
            head = None
            if it.get("pull_request"):
                head = ((forge(f"/pulls/{num}").get("head") or {}).get("ref"))
            if not head:
                if label == "state/submitted":
                    unmatched.append(num)
                continue
            if head not in by_branch:
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
            body = (f"{marker}\n\nUpstream pull request "
                    f"open-goal/jak-project#{up_num} for branch `{head}` is now "
                    f"**{up_state}**, but this item carries `{label}`: {verdict}. "
                    f"Detected by the scheduled reconciliation run; labels are never "
                    f"changed automatically.")
            forge(f"/issues/{num}/comments", "POST", {"body": body})
            print(f"DELTA: forge #{num} [{label}] vs upstream #{up_num} ({up_state})")
            deltas += 1

    if unmatched:
        print(f"state/submitted items with no branch to match (issues, check by hand): "
              f"{sorted(unmatched)}")
    print(f"done: {deltas} delta(s) reported")


if __name__ == "__main__":
    main()

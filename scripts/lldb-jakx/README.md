# lldb helpers for jakx runtime debugging (Windows / titan)

Collected from the slice-5/6 corruption hunt (issue #34). The in-game crash handler
(`game/kernel/common/goal_crash_map.cpp`) now prints a symbolized GOAL crash report on
any fault, so start with a plain boot log before reaching for these.

Prereqs: LLVM's lldb needs `python311.dll` on PATH; the uv-installed runtime works:

```
export PATH="/c/Users/alex/AppData/Roaming/uv/python/cpython-3.11.15-windows-x86_64-none:$PATH"
```

Scripts (load with `command script import <file>`, then use as commands or stop-hooks):

- `stop_dump.py` - `trapinfo2`: at any stop, dumps rip/rsp/argument registers plus every
  stack quadword that looks like a return address. Attach as
  `target stop-hook add --one-liner trapinfo2`.
- `heap_dump.py` - `crashinfo2`: register dump plus a memory window around the store
  target and the pp process name (jakx layout offsets).

Hard-won usage notes:

- Batch mode (`-b`) loses the output of `-o` commands queued after a stop; put
  everything in stop-hooks, which run synchronously at the stop.
- lldb cannot unwind JIT frames. Symbolize GOAL addresses by hand: GOAL addr =
  native addr minus r15; then map against the boot log's `[link and exec] ... 0: 0x...`
  lines (each records the object's GOAL start). The crash handler automates exactly
  this for faults.
- For catch-the-writer problems, arm a hardware watchpoint from inside kscheme's
  process-heap allocator, keyed on the allocation's type name and owner-process name
  (layout-independent); see the DR0 recipe in issue #34's slice-6 notes. Debug-register
  watches fire even in runs that would have survived the corruption.
- **Arm DEFERRED, never at the matching allocation itself.** alloc_from_heap returns
  into alloc_heap_object, which immediately stores the object's own type tag; a watch
  armed inside the allocator always fires on that store, and the capture reads exactly
  like a native memory-copy writer (g_ee_main_mem base in rcx, a GAME.CGO-region value
  in the source register, store-and-advance shape). That false positive was slice-6's
  "caught red-handed" native writer. Record the address as pending and arm it on the
  NEXT alloc_from_heap entry; any hit after that is a genuinely foreign write.
- Crash-report object names use the nearest recorded object start at or below the
  address, and empty 96-byte stub objects share their start with whatever links next
  (script = generic-obs, entity-table = loader, collide-cache = entity, ...). Before
  trusting a `stub+0xNNN` frame, check the boot log's `[link and exec]` spans for the
  object that really contains the address.
- Runs under lldb take 2-3x longer; the ring reaches jungle-b at 7-8 minutes. Boot from
  a nearby continue (`*jakx-boot-continue*` in level-h.gc) when hunting a specific
  section boundary.

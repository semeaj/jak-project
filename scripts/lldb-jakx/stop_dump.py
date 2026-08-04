import lldb
def info(debugger, command, result, internal_dict):
    target = debugger.GetSelectedTarget()
    process = target.GetProcess()
    thread = process.GetSelectedThread()
    frame = thread.GetFrameAtIndex(0)
    out = ["TRAP2 thread=%s stop=%s" % (thread.GetName(), thread.GetStopDescription(64))]
    regs = {}
    for group in frame.GetRegisters():
        for reg in group:
            regs[reg.GetName()] = reg.GetValueAsUnsigned()
    for r in ("rip", "rsp", "rdi", "rsi", "rdx", "r13", "r15"):
        out.append("TRAP2 %s=%#x" % (r, regs.get(r, 0)))
    for i in range(min(thread.GetNumFrames(), 6)):
        f = thread.GetFrameAtIndex(i)
        out.append("TRAP2 frame %d: %#x %s" % (i, f.GetPC(), f.GetFunctionName() or "?"))
    err = lldb.SBError()
    sp = frame.GetSP()
    for delta in range(0, 40):
        v = process.ReadUnsignedFromMemory(sp + delta * 8, 8, err)
        if err.Success() and v > 0x10000000000:
            out.append("TRAP2 ra-candidate [+%03x]=%#x" % (delta * 8, v))
    result.PutCString("\n".join(out))
def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand('command script add -f trapinfo2.info trapinfo2')

import lldb
def info(debugger, command, result, internal_dict):
    target = debugger.GetSelectedTarget()
    process = target.GetProcess()
    thread = process.GetSelectedThread()
    frame = thread.GetFrameAtIndex(0)
    regs = {}
    for group in frame.GetRegisters():
        for reg in group:
            regs[reg.GetName()] = reg.GetValueAsUnsigned()
    r12, r13, r15 = regs.get('r12', 0), regs.get('r13', 0), regs.get('r15', 0)
    out = []
    out.append("CRASHINFO r12=%#x r13=%#x r15=%#x target-mod16=%d" % (r12, r13, r15, r12 % 16))
    err = lldb.SBError()
    if r13:
        name_ptr = process.ReadUnsignedFromMemory(r15 + r13, 4, err)
        if err.Success() and name_ptr:
            nm = process.ReadCStringFromMemory(r15 + name_ptr + 4, 48, err)
            out.append("CRASHINFO pp-name=%r" % nm)
    for delta in range(-32, 33, 4):
        v = process.ReadUnsignedFromMemory(r15 + r12 + delta, 4, err)
        if err.Success():
            out.append("CRASHINFO mem[%+d]=%#010x" % (delta, v))
    # sound player rpc globals: find *sound-player-rpc* symbol value? cheap route: skip
    result.PutCString("\n".join(out))
def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand('command script add -f crashinfo2.info crashinfo2')

#pragma once

/*!
 * @file goal_crash_map.h
 * Crash-time symbolication for GOAL code. The runtime records each linked object's
 * start address in GOAL space; an unhandled-fault handler then prints the faulting
 * address as object+offset, the current GOAL process (name, state, heap usage), and
 * a scan of stack quadwords that land in the GOAL code arena (a poor man's GOAL
 * backtrace). Everything prints to stderr with no allocation, then the fault
 * continues to the default handler / debugger.
 *
 * Motivated by the slice-5/6 jakx bring-up sessions, where reconstructing exactly
 * this information by hand through lldb cost hours per crash (issue #34).
 */

#include "common/common_types.h"

// record one linked object's GOAL-space start (call at link time, before exec)
void goal_crash_map_record(u32 goal_addr, const char* name);

// install the fault handler; call once after GOAL main memory is mapped
void goal_crash_map_install();

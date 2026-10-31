/* TC
 * id:    nfx_stack_resetstkoflw_recovers_probe
 * area:  negative_fatal
 * kind:  positive
 * funcs: _resetstkoflw
 * procs: 
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _resetstkoflw (nonzero on success)
 * note:  PROBES: restoring a captured CONTEXT from a vectored handler after a stack overflow, then _resetstkoflw, makes a second overflow catchable. Child exits 90 on success.
 */
#include "tc.h"
#include <malloc.h>

static CONTEXT saved; static volatile int hits, resumed;
static LONG CALLBACK veh(PEXCEPTION_POINTERS p)
{
    if (p->ExceptionRecord->ExceptionCode != 0xC00000FD) return EXCEPTION_CONTINUE_SEARCH;
    hits++; *p->ContextRecord = saved; return EXCEPTION_CONTINUE_EXECUTION;
}
static int __attribute__((noinline)) rec(volatile int n)
{
    volatile char buf[1024]; buf[0] = (char)n; buf[1023] = (char)n;
    return rec(n + 1) + buf[0] + buf[1023];
}
static int __attribute__((noinline)) child(void)
{
    volatile int phase = 0; volatile int rc = 0;
    AddVectoredExceptionHandler(1, veh);
    saved.ContextFlags = CONTEXT_FULL;
    RtlCaptureContext(&saved);
    if (resumed) {
        rc = _resetstkoflw();
        if (rc == 0) return 91;
        resumed = 0;
        if (hits >= 2) return 90;
    }
    resumed = 1;
    phase = rec(0);
    return 77 + phase;
}
TC_CASE(recovers) { DWORD c = 0; CHECK(tc_spawn_self("r", &c, 10000)); CHECK_EQ_U(c, 90); }
int main(void) { tc_begin(); if (tc_child_name()) return child(); TC_RUN(recovers); return tc_end(); }

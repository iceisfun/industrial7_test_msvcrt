/* TC
 * id:    nfx_neg_signal_sigfpe_int_min_div_probe
 * area:  negative_fatal
 * funcs: signal
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs signal (SIGFPE), _XcptFilter
 * note:  PROBE: the INT_MIN / -1 hardware fault (STATUS_INTEGER_OVERFLOW on x64 idiv) is delivered as SIGFPE to the signal()
 *        handler. Win10 22H2 19045 with the mingw-w64 test exe: the first-chance exception is 0xC0000095 but the child never
 *        terminates (spins until the harness kill, class E); divide-by-zero with the same signal(SIGFPE) handler works (exit
 *        62) and an unhandled-exception filter calling ucrtbase _seh_filter_exe did not help for INT_MIN/-1. Now behind
 *        TC_HAZARD (skipped unless TC_HAZARD=1). Missing datum: the same test from an MSVC-built exe (its startup SEH filter)
 *        or a debugger trace of where the child spins.
 */
#include "tc.h"
#include <signal.h>
#include <limits.h>

static void __cdecl h(int s) { (void)s; ExitProcess(62); }
static volatile int v_a, v_b, v_r;

TC_CASE(int_min)  { DWORD c = 0; TC_HAZARD("INT_MIN / -1 faults (0xC0000095) and the child spins until the 20 s kill timeout on Win10 22H2 19045"); CHECK(tc_spawn_self("min", &c, 20000)); CHECK_EQ_U(c, 62); }

int main(void)
{
    const char *n;
    tc_begin();
    if ((n = tc_child_name()) != NULL) {
        signal(SIGFPE, h);
        (void)n; v_a = INT_MIN; v_b = -1;
        v_r = v_a / v_b;
        return 77;
    }
    TC_RUN(int_min);
    return tc_end();
}

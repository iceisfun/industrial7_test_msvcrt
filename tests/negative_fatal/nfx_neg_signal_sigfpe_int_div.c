/* TC
 * id:    nfx_neg_signal_sigfpe_int_div
 * area:  negative_fatal
 * funcs: signal
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs signal (SIGFPE)
 * note:  Probes: integer divide-by-zero hardware fault is delivered as SIGFPE to the signal() handler.
 *        Toolchain note: test exes are built with mingw-w64, whose CRT startup (not the Microsoft CRT's _XcptFilter) installs the exception-to-signal translation; the CRT under test only stores/returns the signal() handler.
 */
#include "tc.h"
#include <signal.h>
#include <limits.h>

static void __cdecl h(int s) { (void)s; ExitProcess(62); }
static volatile int v_a, v_b, v_r;

TC_CASE(div_zero) { DWORD c = 0; CHECK(tc_spawn_self("zero", &c, 20000)); CHECK_EQ_U(c, 62); }

int main(void)
{
    const char *n;
    tc_begin();
    if ((n = tc_child_name()) != NULL) {
        signal(SIGFPE, h);
        (void)n; v_a = 1; v_b = 0;
        v_r = v_a / v_b;
        return 77;
    }
    TC_RUN(div_zero);
    return tc_end();
}

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
 * note:  Probes: the INT_MIN / -1 hardware fault (STATUS_INTEGER_OVERFLOW on x64 idiv) is delivered as SIGFPE to the signal() handler.
 *        Toolchain note: test exes are built with mingw-w64, whose CRT startup (not the Microsoft CRT's _XcptFilter) installs the exception-to-signal translation; the CRT under test only stores/returns the signal() handler. Whether the mingw translation maps INT_OVERFLOW to SIGFPE is unverified.
 */
#include "tc.h"
#include <signal.h>
#include <limits.h>

static void __cdecl h(int s) { (void)s; ExitProcess(62); }
static volatile int v_a, v_b, v_r;

TC_CASE(int_min)  { DWORD c = 0; CHECK(tc_spawn_self("min", &c, 20000)); CHECK_EQ_U(c, 62); }

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

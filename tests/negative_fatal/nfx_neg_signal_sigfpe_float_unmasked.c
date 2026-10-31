/* TC
 * id:    nfx_neg_signal_sigfpe_float_unmasked
 * area:  negative_fatal
 * funcs: signal _controlfp
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _controlfp, signal (SIGFPE)
 * note:  Probes: unmasked SSE float exceptions (zero-divide, invalid) are delivered as SIGFPE to the signal() handler.
 *        Toolchain note: test exes are built with mingw-w64, whose CRT startup (not the Microsoft CRT's _XcptFilter) installs the exception-to-signal translation; the CRT under test only stores/returns the signal() handler.
 */
#include "tc.h"
#include <signal.h>
#include <float.h>

static void __cdecl h(int s) { (void)s; ExitProcess(63); }
static volatile double v_a, v_b, v_r;

TC_CASE(zerodiv) { DWORD c = 0; CHECK(tc_spawn_self("zdiv", &c, 20000)); CHECK_EQ_U(c, 63); }
TC_CASE(invalid) { DWORD c = 0; CHECK(tc_spawn_self("inv", &c, 20000)); CHECK_EQ_U(c, 63); }

int main(void)
{
    const char *n;
    tc_begin();
    if ((n = tc_child_name()) != NULL) {
        signal(SIGFPE, h);
        v_a = n[0] == 'z' ? 1.0 : 0.0; v_b = 0.0;
        _controlfp(0, n[0] == 'z' ? _EM_ZERODIVIDE : _EM_INVALID);
        v_r = v_a / v_b;
        return 77;
    }
    TC_RUN(zerodiv); TC_RUN(invalid);
    return tc_end();
}

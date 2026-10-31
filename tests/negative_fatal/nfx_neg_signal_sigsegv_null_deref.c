/* TC
 * id:    nfx_neg_signal_sigsegv_null_deref
 * area:  negative_fatal
 * funcs: signal
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs signal (SIGSEGV)
 * note:  Probes: an access violation is translated to SIGSEGV and delivered to the handler installed with signal().
 *        Toolchain note: test exes are built with mingw-w64, whose CRT startup (not the Microsoft CRT's _XcptFilter) installs the exception-to-signal translation; the CRT under test only stores/returns the signal() handler.
 */
#include "tc.h"
#include <signal.h>

static void __cdecl h(int s) { (void)s; ExitProcess(61); }
static volatile char *volatile p;
static volatile char sink;

TC_CASE(write_null) { DWORD c = 0; CHECK(tc_spawn_self("wnull", &c, 20000)); CHECK_EQ_U(c, 61); }
TC_CASE(read_null)  { DWORD c = 0; CHECK(tc_spawn_self("rnull", &c, 20000)); CHECK_EQ_U(c, 61); }
TC_CASE(write_high) { DWORD c = 0; CHECK(tc_spawn_self("whigh", &c, 20000)); CHECK_EQ_U(c, 61); }

int main(void)
{
    const char *n;
    tc_begin();
    if ((n = tc_child_name()) != NULL) {
        signal(SIGSEGV, h);
        if (n[0] == 'w' && n[1] == 'n') { p = NULL; *p = 1; }
        else if (n[0] == 'r') { p = NULL; sink = *p; }
        else { p = (volatile char *)(ULONG_PTR)(~(ULONG_PTR)0xFFFF); *p = 1; }
        return 77;
    }
    TC_RUN(write_null); TC_RUN(read_null); TC_RUN(write_high);
    return tc_end();
}

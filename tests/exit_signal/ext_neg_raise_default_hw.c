/* TC
 * id:    ext_neg_raise_default_hw
 * area:  exit_signal
 * kind:  negative
 * funcs: raise
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   MS docs raise
 * note:  PROBE: unhandled raise(SIGILL/SIGSEGV/SIGFPE) with SIG_DFL terminates with exit code 3, each in its own child.
 */
#include "tc.h"
#include <signal.h>
TC_CASE(sigill)  { DWORD code = 0; CHECK(tc_spawn_self("ill", &code, 20000));  CHECK_EQ_U(code, 3); }
TC_CASE(sigsegv) { DWORD code = 0; CHECK(tc_spawn_self("segv", &code, 20000)); CHECK_EQ_U(code, 3); }
TC_CASE(sigfpe)  { DWORD code = 0; CHECK(tc_spawn_self("fpe", &code, 20000));  CHECK_EQ_U(code, 3); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        volatile int s = (c[0] == 'i') ? SIGILL : (c[0] == 's') ? SIGSEGV : SIGFPE;
        raise(s); return 77;
    }
    TC_RUN(sigill); TC_RUN(sigsegv); TC_RUN(sigfpe);
    return tc_end();
}

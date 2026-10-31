/* TC
 * id:    ext_neg_raise_default_sigint_break
 * area:  exit_signal
 * kind:  negative
 * funcs: raise
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   MS docs raise
 * note:  PROBE: unhandled raise(SIGINT) and raise(SIGBREAK) with SIG_DFL terminate with exit code 3 (not the console ctrl status).
 */
#include "tc.h"
#include <signal.h>
TC_CASE(sigint)   { DWORD code = 0; CHECK(tc_spawn_self("int", &code, 20000));   CHECK_EQ_U(code, 3); }
TC_CASE(sigbreak) { DWORD code = 0; CHECK(tc_spawn_self("brk", &code, 20000)); CHECK_EQ_U(code, 3); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { volatile int s = (c[0] == 'i') ? SIGINT : SIGBREAK; raise(s); return 77; }
    TC_RUN(sigint); TC_RUN(sigbreak);
    return tc_end();
}

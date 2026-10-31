/* TC
 * id:    ext_raise_sig_ign
 * area:  exit_signal
 * funcs: signal raise
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.14.1.1 SIG_IGN; MS docs signal
 */
#include "tc.h"
#include <signal.h>
static void __cdecl h(int s) { (void)s; }
typedef void (__cdecl *sh_t)(int);
static sh_t sg(int n, sh_t f) { return (sh_t)signal(n, f); }

TC_CASE(ign)
{
    CHECK(sg(SIGTERM, SIG_IGN) == SIG_DFL);
    CHECK_EQ_I(raise(SIGTERM), 0);
    CHECK_EQ_I(raise(SIGTERM), 0);   /* still ignored; still alive */
    CHECK(sg(SIGTERM, SIG_DFL) == SIG_IGN);
}
int main(void) { tc_begin(); TC_RUN(ign); return tc_end(); }

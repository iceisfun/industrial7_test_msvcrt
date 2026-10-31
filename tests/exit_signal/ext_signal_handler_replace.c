/* TC
 * id:    ext_signal_handler_replace
 * area:  exit_signal
 * funcs: signal raise
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.14.1.1; MS docs signal
 */
#include "tc.h"
#include <signal.h>
static void __cdecl h(int s) { (void)s; }
typedef void (__cdecl *sh_t)(int);
static sh_t sg(int n, sh_t f) { return (sh_t)signal(n, f); }

static volatile int c1, c2;
static void __cdecl h1(int s) { (void)s; c1++; }
static void __cdecl h2(int s) { (void)s; c2++; }
TC_CASE(replace)
{
    CHECK(sg(SIGTERM, h1) == SIG_DFL);
    CHECK(sg(SIGTERM, h2) == h1);
    CHECK_EQ_I(raise(SIGTERM), 0);
    CHECK_EQ_I(c1, 0);
    CHECK_EQ_I(c2, 1);
    /* the handler may have been reset by the raise, so the return value is not asserted here */
    sg(SIGTERM, SIG_DFL);
}
int main(void) { tc_begin(); TC_RUN(replace); return tc_end(); }

/* TC
 * id:    nfx_raise_sigabrt_handler_returns
 * area:  negative_fatal
 * kind:  positive
 * funcs: raise signal

 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.14.2.1; MS docs raise

 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>

static volatile int flag;
static void __cdecl h(int sig) { (void)sig; flag = 1; }
TC_CASE(returns_zero)
{
    DWORD code = 0; int ran = tc_spawn_self("rr", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 10);
}
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) {
        int r; signal(SIGABRT, h); r = raise(SIGABRT);
        return (r == 0 && flag) ? 10 : 77;
    }
    TC_RUN(returns_zero);
    return tc_end();
}

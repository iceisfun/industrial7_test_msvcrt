/* TC
 * id:    nfx_neg_abort_sigabrt_handler_exit
 * area:  negative_fatal
 * kind:  negative
 * funcs: abort signal

 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs abort, signal: abort raises SIGABRT, user handler is called

 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>

static void __cdecl h(int sig) { (void)sig; _exit(55); }
TC_CASE(handler_runs)
{
    DWORD code = 0; int ran = tc_spawn_self("h", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 55);
}
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) { signal(SIGABRT, h); abort(); return 77; }
    TC_RUN(handler_runs);
    return tc_end();
}

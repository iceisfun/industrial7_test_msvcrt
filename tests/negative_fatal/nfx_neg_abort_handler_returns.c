/* TC
 * id:    nfx_neg_abort_handler_returns
 * area:  negative_fatal
 * kind:  negative
 * funcs: abort signal

 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   MS docs abort
 * note:  PROBE: exit status when the SIGABRT handler returns normally; expected the process still terminates with code 3.
 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>

static void __cdecl h(int sig) { (void)sig; }
TC_CASE(still_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("hret", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK_EQ_U(code, 3);
}
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) { signal(SIGABRT, h); abort(); return 77; }
    TC_RUN(still_terminates);
    return tc_end();
}

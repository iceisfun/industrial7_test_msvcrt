/* TC
 * id:    stl_neg_abort_handler_returns_probe
 * area:  stdlib
 * kind:  negative
 * funcs: abort _set_abort_behavior signal
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs abort
 * note:  Probes that when a SIGABRT handler returns, abort() still terminates the process with exit code 3
 *        (rather than returning to the caller, which would give 77).
 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>

static volatile int hits;
static void __cdecl hret(int sig) { (void)sig; hits++; }

TC_CASE(handler_returns_still_exit3)
{
    DWORD code = 0xffff;
    CHECK(tc_spawn_self("hret", &code, 20000));
    CHECK_EQ_U(code, 3);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { _set_abort_behavior(0, 3); signal(SIGABRT, hret); abort(); return 77; }
    TC_RUN(handler_returns_still_exit3);
    return tc_end();
}

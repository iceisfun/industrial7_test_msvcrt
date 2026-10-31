/* TC
 * id:    nfx_neg_abort_exit_code_3
 * area:  negative_fatal
 * kind:  negative
 * funcs: abort

 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs abort: terminates with exit code 3 (no report fault)
 * note:  Child runs after tc_begin cleared _CALL_REPORTFAULT, so abort() exits with 3 instead of fast-fail.
 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>

TC_CASE(exit3)
{
    DWORD code = 0; int ran = tc_spawn_self("abort", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);          /* abort never returns */
    CHECK_EQ_U(code, 3);
}
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) { abort(); return 77; }
    TC_RUN(exit3);
    return tc_end();
}

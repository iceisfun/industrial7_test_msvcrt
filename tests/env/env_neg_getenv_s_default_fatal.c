/* TC
 * id:    env_neg_getenv_s_default_fatal
 * area:  env
 * funcs: getenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs getenv_s; Parameter validation
 * note:  Without a handler an invalid parameter terminates the process (child process).
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(child_abnormal_exit)
{
    DWORD code = 0; int ran = tc_spawn_self("nullvar", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);   /* 77 = child survived the call */
}
TC_CASE(parent_not_hung)
{
    DWORD code = 0; int ran = tc_spawn_self("nullvar", &code, 20000);
    CHECK(ran);                      /* child ended on its own, not by our timeout kill */
    CHECK(code != 0xdead0002u);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { size_t r; char b[8]; getenv_s(&r, b, sizeof b, NULL); return 77; }
    TC_RUN(child_abnormal_exit); TC_RUN(parent_not_hung);
    return tc_end();
}

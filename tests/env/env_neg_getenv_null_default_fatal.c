/* TC
 * id:    env_neg_getenv_null_default_fatal
 * area:  env
 * kind:  negative
 * funcs: getenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs getenv, Parameter validation: without a handler the invalid parameter terminates the process
 * note:  Child process calls getenv(NULL) with the default handler. Only abnormal termination is asserted (the exact
 *        status, 0xC0000409 on ucrt, is probed elsewhere). Exit 77 means the call returned; 0 means silent success.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(child_exits_abnormally_not_77)
{
    DWORD code = 0; int ran = tc_spawn_self("nullname", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
}
TC_CASE(child_not_zero)
{
    DWORD code = 0; int ran = tc_spawn_self("nullname", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { volatile const char *n = NULL; (void)getenv((const char *)n); return 77; }
    TC_RUN(child_exits_abnormally_not_77); TC_RUN(child_not_zero);
    return tc_end();
}

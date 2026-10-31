/* TC
 * id:    env_neg_putenv_null_default_fatal
 * area:  env
 * kind:  negative
 * funcs: _putenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation": with the default handler _putenv(NULL) terminates the process
 * note:  Child process exits abnormally (non-zero, not our sentinel 77); exact status not asserted.
 */
#include "tc.h"
#include <stdlib.h>

static char *volatile v_null = NULL;

TC_CASE(child_abnormal_exit)
{
    DWORD code = 0; int ran = tc_spawn_self("putenv_null", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
}

int main(void)
{
    tc_begin();
    if (tc_child_name()) { _putenv(v_null); return 77; }
    TC_RUN(child_abnormal_exit);
    return tc_end();
}

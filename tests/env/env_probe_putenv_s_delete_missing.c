/* TC
 * id:    env_probe_putenv_s_delete_missing
 * area:  env
 * funcs: getenv
 * procs: _putenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _putenv_s
 * note:  PROBE: _putenv_s(name, "") for a variable that does not exist. Expect return 0 (deleting nothing is not an error)
 *        and getenv still NULL.
 */
#include "tc.h"
#include <stdlib.h>

TC_DECL_PROC(int, _putenv_s, (const char *, const char *));
static const char *volatile nm = "TCENV_PSM_NEVER_SET";
static const char *volatile vdel = "";

TC_CASE(delete_missing_returns_0)
{
    if (!tc_p__putenv_s) TC_SKIP("_putenv_s not exported");
    CHECK(getenv(nm) == NULL);
    CHECK_EQ_I(tc_p__putenv_s(nm, vdel), 0);
    CHECK(getenv(nm) == NULL);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_putenv_s);
    TC_RUN(delete_missing_returns_0);
    return tc_end();
}

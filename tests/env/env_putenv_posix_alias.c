/* TC
 * id:    env_putenv_posix_alias
 * area:  env
 * funcs: _putenv getenv
 * procs: putenv
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _putenv, _wputenv ("putenv" is a deprecated POSIX name for _putenv)
 * note:  Skips where the DLL does not export putenv (export presence itself is probed in env_probe_putenv_exported). putenv is resolved from the DLL (mingw maps the name to its own static copy).
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

TC_DECL_PROC(int, putenv, (const char *));
static char *volatile v_a   = "TCENV_PX=alias";
static char *volatile v_del = "TCENV_PX=";
static char *volatile v_nm  = "TCENV_PX";
static char *volatile v_b   = "TCENV_PX=viaunderscore";

TC_CASE(putenv_sets_visible_to_getenv)
{
    if (!tc_p_putenv) TC_SKIP("putenv not exported");
    CHECK_EQ_I(tc_p_putenv(v_a), 0);
    CHECK(getenv(v_nm) != NULL);
    if (getenv(v_nm)) CHECK_STR(getenv(v_nm), "alias");
    tc_p_putenv(v_del);
}
TC_CASE(putenv_delete)
{
    if (!tc_p_putenv) TC_SKIP("putenv not exported");
    CHECK_EQ_I(tc_p_putenv(v_a), 0);
    CHECK(getenv(v_nm) != NULL);
    CHECK_EQ_I(tc_p_putenv(v_del), 0);
    CHECK(getenv(v_nm) == NULL);
}
TC_CASE(_putenv_sees_putenv_value)
{
    if (!tc_p_putenv) TC_SKIP("putenv not exported");
    CHECK_EQ_I(tc_p_putenv(v_a), 0);
    CHECK_EQ_I(_putenv(v_b), 0);
    CHECK(getenv(v_nm) != NULL);
    if (getenv(v_nm)) CHECK_STR(getenv(v_nm), "viaunderscore");
    CHECK_EQ_I(_putenv(v_del), 0);
    CHECK(getenv(v_nm) == NULL);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(putenv);
    TC_RUN(putenv_sets_visible_to_getenv);
    TC_RUN(putenv_delete);
    TC_RUN(_putenv_sees_putenv_value);
    return tc_end();
}

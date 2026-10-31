/* TC
 * id:    env_putenv_s_empty_deletes
 * area:  env
 * funcs: getenv
 * procs: _putenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _putenv_s ("you can remove a variable ... by specifying an empty string for value")
 * note:  Deleting a variable that does not exist is covered by env_probe_putenv_s_delete_missing.
 */
#include "tc.h"
#include <stdlib.h>

TC_DECL_PROC(int, _putenv_s, (const char *, const char *));

static const char *volatile nm = "TCENV_PSD";
static const char *volatile v1 = "val";
static const char *volatile v2 = "again";
static const char *volatile vdel = "";

TC_CASE(delete_existing_returns_0)
{
    if (!tc_p__putenv_s) TC_SKIP("_putenv_s not exported");
    CHECK_EQ_I(tc_p__putenv_s(nm, v1), 0);
    CHECK_EQ_I(tc_p__putenv_s(nm, vdel), 0);
}
TC_CASE(getenv_null_after)
{
    if (!tc_p__putenv_s) TC_SKIP("_putenv_s not exported");
    CHECK_EQ_I(tc_p__putenv_s(nm, v1), 0);
    CHECK(getenv(nm) != NULL);
    tc_p__putenv_s(nm, vdel);
    CHECK(getenv(nm) == NULL);
}
TC_CASE(readd_after_delete)
{
    if (!tc_p__putenv_s) TC_SKIP("_putenv_s not exported");
    tc_p__putenv_s(nm, v1);
    tc_p__putenv_s(nm, vdel);
    CHECK_EQ_I(tc_p__putenv_s(nm, v2), 0);
    CHECK(getenv(nm) != NULL);
    if (getenv(nm)) CHECK_STR(getenv(nm), "again");
    tc_p__putenv_s(nm, vdel);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_putenv_s);
    TC_RUN(delete_existing_returns_0); TC_RUN(getenv_null_after); TC_RUN(readd_after_delete);
    return tc_end();
}

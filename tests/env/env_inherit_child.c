/* TC
 * id:    env_inherit_child
 * area:  env
 * funcs: _putenv getenv _putenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  observed
 * ref:   process environment inheritance (CreateProcess with NULL lpEnvironment); _putenv updates the process environment block
 */
#include "tc.h"
#include <stdlib.h>

static int child_main(const char *name)
{
    if (tc_strequ(name, "see")) {
        const char *v = getenv("TC_INH_A");
        return (v && tc_strequ(v, "from-parent")) ? 0 : 10;
    }
    if (tc_strequ(name, "missing"))
        return getenv("TC_INH_B") == NULL ? 0 : 11;
    if (tc_strequ(name, "set"))
        return (_putenv_s("TC_INH_C", "from-child") == 0 && getenv("TC_INH_C")) ? 0 : 12;
    return 99;
}

TC_CASE(child_sees_put)
{
    DWORD code = 1;
    CHECK_EQ_I(_putenv("TC_INH_A=from-parent"), 0);
    CHECK(tc_spawn_self("see", &code, 20000));
    CHECK_EQ_U(code, 0);
    _putenv("TC_INH_A=");
}
TC_CASE(child_sees_deleted_missing)
{
    DWORD code = 1;
    CHECK_EQ_I(_putenv("TC_INH_B=temp"), 0);
    CHECK_EQ_I(_putenv("TC_INH_B="), 0);
    CHECK(tc_spawn_self("missing", &code, 20000));
    CHECK_EQ_U(code, 0);
}
TC_CASE(child_put_not_in_parent)
{
    DWORD code = 1;
    CHECK(tc_spawn_self("set", &code, 20000));
    CHECK_EQ_U(code, 0);
    CHECK(getenv("TC_INH_C") == NULL);
    CHECK_EQ_I(GetEnvironmentVariableA("TC_INH_C", (char[4]){0}, 4), 0);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(child_sees_put); TC_RUN(child_sees_deleted_missing); TC_RUN(child_put_not_in_parent);
    return tc_end();
}

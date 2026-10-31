/* TC
 * id:    env_getenv_empty_name_value
 * area:  env
 * funcs: getenv _putenv
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs getenv
 * note:  Edge-case names. getenv("") returning NULL is what Windows CRTs do (no variable has an empty name).
 *        Names containing '=' can never match since stored entries split at the first '='.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(getenv_empty_name_returns_null) { volatile const char *n = ""; CHECK(getenv((const char *)n) == NULL); }
TC_CASE(name_containing_equals_not_found)
{
    _putenv("TCV_EQ=xyz");
    CHECK(getenv("TCV_EQ=xyz") == NULL);
    CHECK(getenv("TCV_EQ=") == NULL);
    CHECK(getenv("TCV_EQ=x") == NULL);
    CHECK(getenv("TCV_EQ") != NULL);
    _putenv("TCV_EQ=");
}
TC_CASE(name_with_space_found)
{
    char *v;
    _putenv("TCV SP=spaced");
    v = getenv("TCV SP"); CHECK(v != NULL); if (v) CHECK_STR(v, "spaced");
    _putenv("TCV SP=");
}
TC_CASE(trailing_space_in_name_distinct)
{
    char *v;
    _putenv("TCV_TS=plain");
    CHECK(getenv("TCV_TS ") == NULL);
    v = getenv("TCV_TS"); CHECK(v != NULL); if (v) CHECK_STR(v, "plain");
    _putenv("TCV_TS=");
}

int main(void)
{
    tc_begin();
    TC_RUN(getenv_empty_name_returns_null); TC_RUN(name_containing_equals_not_found);
    TC_RUN(name_with_space_found); TC_RUN(trailing_space_in_name_distinct);
    return tc_end();
}

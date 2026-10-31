/* TC
 * id:    stl_getenv_putenv_basic
 * area:  stdlib
 * funcs: getenv _putenv
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs getenv, _putenv (an empty value removes the variable; getenv search is case-insensitive on Windows)
 * note:  Process-local environment only; the variable is removed at the end.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(roundtrip)
{
    CHECK_EQ_I(_putenv("TC_STL_V=hello"), 0);
    CHECK_STR(getenv("TC_STL_V"), "hello");
}
TC_CASE(overwrite)
{
    CHECK_EQ_I(_putenv("TC_STL_V=first"), 0);
    CHECK_EQ_I(_putenv("TC_STL_V=second-longer"), 0);
    CHECK_STR(getenv("TC_STL_V"), "second-longer");
    CHECK_EQ_I(_putenv("TC_STL_V=x"), 0);
    CHECK_STR(getenv("TC_STL_V"), "x");
}
TC_CASE(remove)
{
    _putenv("TC_STL_V=abc");
    CHECK(getenv("TC_STL_V") != NULL);
    CHECK_EQ_I(_putenv("TC_STL_V="), 0);
    CHECK(getenv("TC_STL_V") == NULL);
}
TC_CASE(unknown)
{
    CHECK(getenv("TC_STL_DOES_NOT_EXIST_9") == NULL);
}
TC_CASE(case_insensitive)
{
    _putenv("TC_STL_V=cs");
    CHECK_STR(getenv("tc_stl_v"), "cs");
    CHECK_STR(getenv("Tc_Stl_V"), "cs");
}
int main(void)
{
    tc_begin();
    TC_RUN(roundtrip); TC_RUN(overwrite); TC_RUN(remove); TC_RUN(unknown); TC_RUN(case_insensitive);
    _putenv("TC_STL_V=");
    return tc_end();
}

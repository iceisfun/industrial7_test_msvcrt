/* TC
 * id:    env_getenv_case_insensitive
 * area:  env
 * funcs: getenv _putenv
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs getenv (the name is case-insensitive on Windows, the value is case-sensitive), _putenv
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(lower_finds_upper)
{
    char *v;
    _putenv("TCV_CI_UP=Value");
    v = getenv("tcv_ci_up"); CHECK(v != NULL); if (v) CHECK_STR(v, "Value");
    _putenv("TCV_CI_UP=");
}
TC_CASE(upper_finds_lower)
{
    char *v;
    _putenv("tcv_ci_lo=Value");
    v = getenv("TCV_CI_LO"); CHECK(v != NULL); if (v) CHECK_STR(v, "Value");
    _putenv("tcv_ci_lo=");
}
TC_CASE(mixed_case)
{
    char *v;
    _putenv("TcV_MiXeD=mm");
    v = getenv("tCv_mIxEd"); CHECK(v != NULL); if (v) CHECK_STR(v, "mm");
    v = getenv("TCV_MIXED"); CHECK(v != NULL); if (v) CHECK_STR(v, "mm");
    _putenv("TcV_MiXeD=");
}
TC_CASE(value_case_preserved)
{
    char *v;
    _putenv("TCV_VC=MiXeD CaSe");
    v = getenv("tcv_vc"); CHECK(v != NULL); if (v) CHECK_STR(v, "MiXeD CaSe");
    _putenv("TCV_VC=");
}

int main(void)
{
    tc_begin();
    TC_RUN(lower_finds_upper); TC_RUN(upper_finds_lower); TC_RUN(mixed_case); TC_RUN(value_case_preserved);
    return tc_end();
}

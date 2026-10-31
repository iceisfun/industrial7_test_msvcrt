/* TC
 * id:    err_neg_param_sprintf_s_small
 * area:  errno_param
 * kind:  negative
 * funcs: sprintf_s _set_invalid_parameter_handler
 * crt:   >=100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf_s
 * note:  msvcr80/90 omitted: mingw import libs lack sprintf_s. ucrtbase satisfies sprintf_s via __stdio_common_vsprintf_s.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(small_buffer)
{
    char b[4] = { 'x', 'x', 'x', 'x' }; int r; volatile int v = 123456; tc_iph_count = 0; errno = 0;
    r = sprintf_s(b, sizeof b, "%d", v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    char b[8] = "q"; int r; const char *volatile f = NULL; tc_iph_count = 0; errno = 0;
    r = sprintf_s(b, sizeof b, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buffer)
{
    int r; char *volatile p = NULL; tc_iph_count = 0; errno = 0;
    r = sprintf_s(p, 8, "abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(exact_fit_ok)
{
    char b[4]; int r; volatile int v = 123; tc_iph_count = 0;
    r = sprintf_s(b, sizeof b, "%d", v);
    CHECK_EQ_I(r, 3); CHECK_STR(b, "123"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no handler API\n"); return 1; }
    TC_RUN(small_buffer); TC_RUN(null_format); TC_RUN(null_buffer); TC_RUN(exact_fit_ok);
    /* three failing calls above -> three handler invocations in total is checked per case */
    return tc_end();
}

/* TC
 * id:    err_neg_param_wcscpy_s_errno
 * area:  errno_param
 * kind:  negative
 * funcs: wcscpy_s _set_invalid_parameter_handler
 * crt:   >=100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs wcscpy_s
 * note:  msvcr80/90 omitted: mingw import libs lack wcscpy_s.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

TC_CASE(null_dest)
{
    errno_t r; tc_iph_count = 0; errno = 0;
    r = wcscpy_s(NULL, 8, L"abc");
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size_zero)
{
    wchar_t d[4] = { L'q', 0, 0, 0 }; errno_t r; tc_iph_count = 0; errno = 0;
    r = wcscpy_s(d, 0, L"abc");
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(too_small)
{
    wchar_t d[3] = { L'x', L'x', L'x' }; errno_t r; tc_iph_count = 0; errno = 0;
    r = wcscpy_s(d, 3, L"abcdef");
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(d[0], 0);
}
TC_CASE(null_src)
{
    wchar_t d[3] = { L'x', L'x', L'x' }; errno_t r; tc_iph_count = 0; errno = 0;
    r = wcscpy_s(d, 3, NULL);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(d[0], 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no handler API\n"); return 1; }
    TC_RUN(null_dest); TC_RUN(size_zero); TC_RUN(too_small); TC_RUN(null_src);
    return tc_end();
}

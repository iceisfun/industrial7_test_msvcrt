/* TC
 * id:    err_neg_param_mbstowcs_s_small
 * area:  errno_param
 * kind:  negative
 * funcs: mbstowcs_s _set_invalid_parameter_handler
 * crt:   >=100
 * tier:  p2
 * conf:  documented
 * ref:   MS docs mbstowcs_s, _TRUNCATE
 * note:  msvcr80/90 omitted: mingw import libs lack mbstowcs_s. C locale, ASCII input only.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

#ifndef STRUNCATE
#define STRUNCATE 80
#endif

TC_CASE(dest_too_small)
{
    wchar_t d[3] = { L'x', L'x', L'x' }; size_t n = 99; errno_t r; volatile size_t cnt = 10; tc_iph_count = 0;
    r = mbstowcs_s(&n, d, 3, "abcdef", cnt);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(dest_null_size_nonzero)
{
    size_t n = 99; errno_t r; wchar_t *volatile d = NULL; tc_iph_count = 0;
    r = mbstowcs_s(&n, d, 4, "abc", 3);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(truncate_flag)
{
    wchar_t d[4] = { 1, 1, 1, 1 }; size_t n = 99; errno_t r; volatile size_t cnt = _TRUNCATE; tc_iph_count = 0;
    r = mbstowcs_s(&n, d, 4, "abcdefgh", cnt);
    CHECK_EQ_I(r, STRUNCATE);
    CHECK_WSTR(d, L"abc");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no handler API\n"); return 1; }
    TC_RUN(dest_too_small); TC_RUN(dest_null_size_nonzero); TC_RUN(truncate_flag);
    return tc_end();
}

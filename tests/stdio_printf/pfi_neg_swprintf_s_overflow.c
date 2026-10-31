/* TC
 * id:    pfi_neg_swprintf_s_overflow
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs swprintf_s, vswprintf_s: sizeInWords validation, ERANGE/EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(small_buffer)
{
    wchar_t b[8]; int n; volatile int v = 12345; b[0] = L'x'; errno = 0; tc_iph_count = 0;
    n = swprintf_s(b, 4, L"%d", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(null_buffer)
{
    wchar_t *volatile p = NULL; int n; errno = 0; tc_iph_count = 0;
    n = swprintf_s(p, 8, L"%d", 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    wchar_t b[8]; const wchar_t *volatile f = NULL; int n; errno = 0; tc_iph_count = 0;
    n = swprintf_s(b, 8, f, 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(zero_size)
{
    wchar_t b[8]; volatile size_t z = 0; int n; errno = 0; tc_iph_count = 0;
    n = swprintf_s(b, z, L"%d", 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(exact_fit)
{
    wchar_t b[4]; int n; volatile int v = 123; tc_iph_count = 0;
    n = swprintf_s(b, 4, L"%d", v);
    CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"123"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(small_buffer); TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(zero_size); TC_RUN(exact_fit);
    return tc_end();
}

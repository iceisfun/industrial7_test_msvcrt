/* TC
 * id:    pfi_neg_snwprintf_s_truncate
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snwprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snwprintf_s, _snwprintf_s_l
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(truncate)
{
    wchar_t b[8]; int n, i; volatile int v = 1234567;
    for (i = 0; i < 8; i++) b[i] = L'Z';
    tc_iph_count = 0;
    n = _snwprintf_s(b, 5, _TRUNCATE, L"%d", v);
    CHECK_EQ_I(n, -1); CHECK_WSTR(b, L"1234"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(null_buffer)
{
    wchar_t *volatile p = NULL; int n; errno = 0; tc_iph_count = 0;
    n = _snwprintf_s(p, 8, 4, L"%d", 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    wchar_t b[8]; const wchar_t *volatile f = NULL; int n; errno = 0; tc_iph_count = 0;
    n = _snwprintf_s(b, 8, 4, f, 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(overflow)
{
    wchar_t b[4]; int n; volatile int v = 1234567; b[0] = L'x'; errno = 0; tc_iph_count = 0;
    n = _snwprintf_s(b, 4, 10, L"%d", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(truncate); TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(overflow);
    return tc_end();
}

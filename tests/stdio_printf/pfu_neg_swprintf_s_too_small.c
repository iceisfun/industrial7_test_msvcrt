/* TC
 * id:    pfu_neg_swprintf_s_too_small
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, swprintf_s: size in elements; too small -> handler, buffer[0]=0, -1; NULL buffer/format -> EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static wchar_t f_abcd[] = L"abcd";
static wchar_t f_x[] = L"x";

TC_CASE(too_small)
{
    wchar_t w[16]; volatile size_t n = 4; int r, i;
    for (i = 0; i < 16; i++) w[i] = L'Q';
    tc_iph_count = 0;
    r = swprintf_s(w, n, f_abcd);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(w[0], 0);
}
TC_CASE(exact_fit)
{
    wchar_t w[16]; volatile size_t n = 5; int r;
    tc_iph_count = 0;
    r = swprintf_s(w, n, f_abcd);
    CHECK_EQ_I(r, 4);
    CHECK_WSTR(w, L"abcd");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(null_buffer)
{
    wchar_t *volatile p = NULL; volatile size_t n = 8; int r;
    tc_iph_count = 0; errno = 0;
    r = swprintf_s(p, n, f_x);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    wchar_t w[8]; volatile size_t n = 8; const wchar_t *volatile f = NULL; int r;
    w[0] = L'Q';
    tc_iph_count = 0; errno = 0;
    r = swprintf_s(w, n, f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(too_small); TC_RUN(exact_fit);  TC_RUN(null_buffer); TC_RUN(null_format);
    return tc_end();
}

/* TC
 * id:    pfu_neg_snwprintf_s_truncate
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snwprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s, _snprintf_s_l, _snwprintf_s: count<len truncates and returns -1 (no handler); _TRUNCATE fills size-1 and returns -1; count>=size otherwise handler
 * note:  msvcrt.dll: the invalid-parameter assertions are guarded with TC_AT_LEAST(80); the msvcrt only gets the truncation paths.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
static wchar_t f_7[] = L"abcdefg";
static wchar_t f_6[] = L"abcdef";
static wchar_t f_4[] = L"abcd";

TC_CASE(truncate_flag)
{
    wchar_t w[16]; volatile size_t n = 5, c = _TRUNCATE; int r, i;
    for (i = 0; i < 16; i++) w[i] = L'Q';
    tc_iph_count = 0;
    r = _snwprintf_s(w, n, c, f_7);
    CHECK_EQ_I(r, -1);
    CHECK_WSTR(w, L"abcd");
    CHECK_EQ_I(w[5], L'Q');
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_less_than_len)
{
    wchar_t w[16]; volatile size_t n = 16, c = 2; int r, i;
    for (i = 0; i < 16; i++) w[i] = L'Q';
    tc_iph_count = 0;
    r = _snwprintf_s(w, n, c, f_6);
    CHECK_EQ_I(r, -1);
    CHECK_WSTR(w, L"ab");
    CHECK_EQ_I(w[3], L'Q');
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_ge_size_overflow)
{
    wchar_t w[16]; volatile size_t n = 4, c = 10; int r, i;
    if (!TC_AT_LEAST(80)) TC_SKIP("no invalid parameter handler");
    for (i = 0; i < 16; i++) w[i] = L'Q';
    tc_iph_count = 0;
    r = _snwprintf_s(w, n, c, f_7);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(w[0], 0);
}
TC_CASE(truncate_exact_fit)
{
    wchar_t w[16]; volatile size_t n = 5, c = _TRUNCATE; int r;
    tc_iph_count = 0;
    r = _snwprintf_s(w, n, c, f_4);
    CHECK_EQ_I(r, 4);
    CHECK_WSTR(w, L"abcd");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(truncate_flag); TC_RUN(count_less_than_len); TC_RUN(count_ge_size_overflow); TC_RUN(truncate_exact_fit);
    return tc_end();
}

/* TC
 * id:    pfl_neg_snwprintf_l_truncate
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsnwprintf_l _snwprintf_s_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf, _snprintf_s, _snprintf_s_l, _snwprintf_s_l, _TRUNCATE; "Parameter validation"
 * note:  _snwprintf_l (via _vsnwprintf_l / ucrt inline): output longer than count -> -1 and no terminator.
 *        _snwprintf_s_l: _TRUNCATE -> truncates, always NUL terminates, returns -1; size 0 -> -1 without writing; NULL buffer -> invalid parameter, EINVAL;
 *        count >= size without _TRUNCATE and not fitting -> invalid parameter, ERANGE, buffer[0] = 0.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
#include <locale.h>

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif

static int snw(wchar_t *b, size_t n, const wchar_t *f, _locale_t l, ...)
{
    va_list ap; int r;
    va_start(ap, l);
#if TC_CRT_VER >= 140
    r = __stdio_common_vswprintf(5ULL, b, n, f, l, ap);
#else
    r = _vsnwprintf_l(b, n, f, l, ap);
#endif
    va_end(ap);
    return r;
}

static _locale_t loc;

TC_CASE(snwprintf_l_trunc_ret_minus1)
{
    wchar_t b[16]; int i, r;
    for (i = 0; i < 16; i++) b[i] = L'Z';
    r = snw(b, 4, L"abcdefgh", loc);
    CHECK_EQ_I(r, -1);
    CHECK(b[0] == L'a' && b[1] == L'b' && b[2] == L'c' && b[3] == L'd');
    CHECK(b[4] == L'Z');  /* nothing written past count, and no terminator added */
}
TC_CASE(snwprintf_s_l_truncate_flag_ret_minus1_terminated)
{
    wchar_t b[16]; int i, r;
    for (i = 0; i < 16; i++) b[i] = L'Z';
    errno = 0; tc_iph_count = 0;
    r = _snwprintf_s_l(b, 4, _TRUNCATE, L"abcdefgh", loc);
    CHECK_EQ_I(r, -1);
    CHECK_WSTR(b, L"abc");
    CHECK(b[4] == L'Z');
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(snwprintf_s_l_overflow_iph_erange)
{
    wchar_t b[8]; int r;
    b[0] = L'Q'; tc_iph_count = 0; errno = 0;
    r = _snwprintf_s_l(b, 4, 8, L"abcdefgh", loc);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, ERANGE);
    CHECK(b[0] == 0);
}
TC_CASE(snwprintf_s_l_size_zero_ret_minus1)
{
    wchar_t b[8]; int r;
    b[0] = L'Q'; tc_iph_count = 0; errno = 0;
    r = _snwprintf_s_l(b, 0, 4, L"ab", loc);
    CHECK_EQ_I(r, -1);
    CHECK(b[0] == L'Q');   /* nothing written; the errno/handler details for size 0 are not asserted */
}
TC_CASE(snwprintf_s_l_null_buf_iph)
{
    wchar_t *volatile p = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = _snwprintf_s_l(p, 8, 4, L"ab", loc);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::main _create_locale(C) failed\n"); return 1; }
    TC_RUN(snwprintf_l_trunc_ret_minus1); TC_RUN(snwprintf_s_l_truncate_flag_ret_minus1_terminated);
    TC_RUN(snwprintf_s_l_overflow_iph_erange); TC_RUN(snwprintf_s_l_size_zero_ret_minus1); TC_RUN(snwprintf_s_l_null_buf_iph);
    _free_locale(loc);
    return tc_end();
}

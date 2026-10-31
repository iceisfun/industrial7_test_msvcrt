/* TC
 * id:    pfl_neg_swprintf_s_l_small_state
 * area:  stdio_printf
 * funcs: _swprintf_s_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs swprintf_s, _swprintf_s_l (buffer too small: invalid parameter handler, ERANGE, return -1, buffer[0] = 0)
 * note:  Wide twin of pfl_neg_sprintf_s_l_small. Exact fit is n-1 characters plus the terminator.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>
#include <locale.h>

#define G L'\x7777'
static wchar_t * volatile v_fmt;

TC_CASE(too_small)
{
    _locale_t l = _create_locale(LC_ALL, "C"); wchar_t b[7]; int r, i;
    CHECK(l != NULL); if (!l) return;
    for (i = 0; i < 7; i++) b[i] = L'X';
    tc_iph_count = 0; errno = 0;
    r = _swprintf_s_l(b, 5, L"%ls", l, L"hello");   /* needs 6 with the terminator */
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
    CHECK_EQ_I(b[5], L'X'); CHECK_EQ_I(b[6], L'X');   /* beyond the stated size untouched */
    _free_locale(l);
}
TC_CASE(exact_fit)
{
    _locale_t l = _create_locale(LC_ALL, "C"); wchar_t b[7]; int r, i;
    CHECK(l != NULL); if (!l) return;
    for (i = 0; i < 7; i++) b[i] = G;
    tc_iph_count = 0;
    r = _swprintf_s_l(b, 6, L"%ls", l, L"hello");
    CHECK_EQ_I(r, 5); CHECK_WSTR(b, L"hello"); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(b[6], G);
    _free_locale(l);
}
TC_CASE(one_over_exact)
{
    _locale_t l = _create_locale(LC_ALL, "C"); wchar_t b[7]; int r;
    CHECK(l != NULL); if (!l) return;
    b[0] = L'Q'; tc_iph_count = 0; errno = 0;
    r = _swprintf_s_l(b, 5, L"%ls", l, L"hello");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(b[0], 0);
    r = _swprintf_s_l(b, 5, L"%ls", l, L"hell");
    CHECK_EQ_I(r, 4); CHECK_WSTR(b, L"hell"); CHECK_EQ_I(tc_iph_count, 1);
    _free_locale(l);
}
TC_CASE(null_fmt_and_buf)
{
    _locale_t l = _create_locale(LC_ALL, "C"); wchar_t b[4]; int r;
    CHECK(l != NULL); if (!l) return;
    v_fmt = NULL; tc_iph_count = 0; errno = 0;
    r = _swprintf_s_l(b, 4, v_fmt, l);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0;
    r = _swprintf_s_l(NULL, 4, L"x", l);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    _free_locale(l);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(too_small); TC_RUN(exact_fit); TC_RUN(one_over_exact); TC_RUN(null_fmt_and_buf);
    return tc_end();
}

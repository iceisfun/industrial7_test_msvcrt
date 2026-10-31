/* TC
 * id:    pfl_swprintf_l_wide
 * area:  stdio_printf
 * funcs: _vsnwprintf_l _swprintf_s_l _create_locale _free_locale
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf, _snwprintf, _sprintf_s_l, _swprintf_s_l; printf width specification (%s/%hs/%ls in wide functions)
 * note:  In MS wide printf functions %s = wide string, %hs = narrow string, %ls = wide string. German cases skip if absent.
 *        _swprintf_l is not used (legacy vs standard signature differs between CRT generations).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <locale.h>

/* ucrtbase has no _vsnwprintf_l export (the SDK inlines it over __stdio_common_vswprintf with options
 * LEGACY_WIDE_SPECIFIERS(4)|LEGACY_VSPRINTF_NULL_TERMINATION(1), the defaults without _CRT_STDIO_ISO_WIDE_SPECIFIERS). */
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

TC_CASE(snwprintf_l_double_c)
{
    _locale_t l = _create_locale(LC_ALL, "C"); wchar_t b[32]; volatile double d = 3.25; int r;
    CHECK(l != NULL); if (!l) return;
    r = snw(b, 32, L"%.2f", l, d);
    CHECK_EQ_I(r, 4); CHECK_WSTR(b, L"3.25");
    _free_locale(l);
}
TC_CASE(snwprintf_l_double_de)
{
    _locale_t l = _create_locale(LC_ALL, "German_Germany.1252"); wchar_t b[32]; volatile double d = 3.25; int r;
    if (!l) TC_SKIP("German locale not installed");
    r = snw(b, 32, L"%.2f", l, d);
    CHECK_EQ_I(r, 4); CHECK_WSTR(b, L"3,25");
    _free_locale(l);
}
TC_CASE(swprintf_s_l_ok)
{
    _locale_t l = _create_locale(LC_ALL, "C"); wchar_t b[16]; int r; volatile int v = -17;
    CHECK(l != NULL); if (!l) return;
    r = _swprintf_s_l(b, 16, L"[%4d]", l, v);
    CHECK_EQ_I(r, 6); CHECK_WSTR(b, L"[ -17]");
    _free_locale(l);
}
TC_CASE(hs_ls_widths)
{
    _locale_t l = _create_locale(LC_ALL, "C"); wchar_t b[64]; int r;
    CHECK(l != NULL); if (!l) return;
    r = _swprintf_s_l(b, 64, L"%hs|%ls|%s|%S", l, "nar", L"wid", L"w2", "n2");
    CHECK_WSTR(b, L"nar|wid|w2|n2"); CHECK_EQ_I(r, 13);
    _free_locale(l);
}
TC_CASE(return_wchar_count)
{
    _locale_t l = _create_locale(LC_ALL, "C"); wchar_t b[64]; int r;
    CHECK(l != NULL); if (!l) return;
    r = snw(b, 64, L"%ls%d", l, L"abc\x00e4", 12345);
    CHECK_EQ_I(r, 9); /* characters, not bytes */
    _free_locale(l);
}

int main(void)
{
    tc_begin();
    TC_RUN(snwprintf_l_double_c); TC_RUN(snwprintf_l_double_de); TC_RUN(swprintf_s_l_ok); TC_RUN(hs_ls_widths); TC_RUN(return_wchar_count);
    return tc_end();
}

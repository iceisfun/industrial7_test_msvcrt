/* TC
 * id:    pfl_vswprintf_l_wide
 * area:  stdio_printf
 * funcs: _vsnwprintf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _vsnwprintf_l; vswprintf family
 * note:  The wide va_list variant with locale (_vsnwprintf_l; the _vswprintf_l signature differs between CRT generations).
 *        On ucrtbase the SDK inlines it over __stdio_common_vswprintf(5). German case skips if the locale is absent.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <locale.h>

static int vw(wchar_t *b, size_t n, _locale_t l, const wchar_t *f, ...)
{
    va_list ap; int r;
    va_start(ap, f);
#if TC_CRT_VER >= 140
    r = __stdio_common_vswprintf(5ULL, b, n, f, l, ap);
#else
    r = _vsnwprintf_l(b, n, f, l, ap);
#endif
    va_end(ap);
    return r;
}

TC_CASE(vswprintf_l_wide_c)
{
    _locale_t l = _create_locale(LC_ALL, "C"); wchar_t b[32]; int r; volatile double d = 0.5;
    CHECK(l != NULL); if (!l) return;
    r = vw(b, 32, l, L"%d|%.1f|%ls", 3, d, L"w");
    CHECK_WSTR(b, L"3|0.5|w"); CHECK_EQ_I(r, 7);
    _free_locale(l);
}
TC_CASE(vswprintf_l_wide_de)
{
    _locale_t l = _create_locale(LC_ALL, "German_Germany.1252"); wchar_t b[32]; int r; volatile double d = 0.5;
    if (!l) TC_SKIP("German locale not installed");
    r = vw(b, 32, l, L"%d|%.1f|%ls", 3, d, L"w");
    CHECK_WSTR(b, L"3|0,5|w"); CHECK_EQ_I(r, 7);
    _free_locale(l);
}

int main(void)
{
    tc_begin();
    TC_RUN(vswprintf_l_wide_c); TC_RUN(vswprintf_l_wide_de);
    return tc_end();
}

/* TC
 * id:    pfu_vsnprintf_s_l_locale
 * area:  stdio_printf
 * funcs: _vsnprintf_s_l _vsnwprintf_s_l _vsprintf_s_l _vswprintf_s_l
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf_s, _sprintf_s_l, swprintf_s, _swprintf_s_l; _snprintf_s, _vsnprintf_s family (explicit _locale_t parameter)
 * note:  msvcrt.dll is not listed: the Windows 7 msvcrt.dll does not export the _l secure forms. A NULL locale means the current locale (C here).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <locale.h>
#include <errno.h>

static _locale_t loc;

static int a_vsprintf(char *b, size_t n, _locale_t l, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsprintf_s_l(b, n, f, l, ap); va_end(ap); return r; }
static int a_vsnprintf(char *b, size_t n, size_t c, _locale_t l, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnprintf_s_l(b, n, c, f, l, ap); va_end(ap); return r; }
static int a_vswprintf(wchar_t *b, size_t n, _locale_t l, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vswprintf_s_l(b, n, f, l, ap); va_end(ap); return r; }
static int a_vsnwprintf(wchar_t *b, size_t n, size_t c, _locale_t l, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnwprintf_s_l(b, n, c, f, l, ap); va_end(ap); return r; }

TC_CASE(c_locale_float)
{
    char b[32]; volatile double d = 2.5; int r;
    r = a_vsprintf(b, sizeof b, loc, "%.1f", d);
    CHECK_EQ_I(r, 3); CHECK_STR(b, "2.5");
    r = a_vsnprintf(b, sizeof b, 10, loc, "%.1f|%d", d, 7);
    CHECK_EQ_I(r, 5); CHECK_STR(b, "2.5|7");
}
TC_CASE(null_locale)
{
    char b[32]; volatile double d = 1.25; int r;
    r = a_vsprintf(b, sizeof b, NULL, "%.2f", d);
    CHECK_EQ_I(r, 4); CHECK_STR(b, "1.25");
    r = a_vsnprintf(b, sizeof b, 8, NULL, "%s", "xyz");
    CHECK_EQ_I(r, 3); CHECK_STR(b, "xyz");
}
TC_CASE(snprintf_truncation)
{
    char b[16]; volatile size_t n = 16, c = 3; int r;
    tc_iph_count = 0;
    r = a_vsnprintf(b, n, c, loc, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_basic)
{
    wchar_t b[32]; int r;
    r = a_vswprintf(b, 32, loc, L"%d-%ls", 12, L"ab");
    CHECK_EQ_I(r, 5); CHECK_WSTR(b, L"12-ab");
    r = a_vsnwprintf(b, 32, 4, loc, L"%ls", L"abcdefg");
    CHECK_EQ_I(r, -1); CHECK_WSTR(b, L"abcd");
    r = a_vsnwprintf(b, 32, 8, NULL, L"%.1f", 3.5);
    CHECK_EQ_I(r, 3); CHECK_WSTR(b, L"3.5");
}
int main(void)
{
    tc_begin(); tc_iph_install();
    loc = _create_locale(LC_ALL, "C"); CHECK(loc != NULL);
    TC_RUN(c_locale_float); TC_RUN(null_locale); TC_RUN(snprintf_truncation); TC_RUN(wide_basic);
    _free_locale(loc);
    return tc_end();
}

/* TC
 * id:    pfq_sprintf_p_l_locale
 * area:  stdio_printf
 * funcs: _sprintf_p_l _swprintf_p_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p positional parameters; _sprintf_p, _sprintf_p_l, _swprintf_p
 * note:  C locale only.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <stdarg.h>

#ifdef _UCRT
/* mingw's UCRT headers do not declare the wide _p family; real ucrtbase implements them inline over __stdio_common_vswprintf_p. */
int __cdecl __stdio_common_vswprintf_p(unsigned __int64, wchar_t *, size_t, const wchar_t *, _locale_t, va_list);
static int _vswprintf_p_l(wchar_t *b, size_t n, const wchar_t *f, _locale_t l, va_list ap) { return __stdio_common_vswprintf_p(0, b, n, f, l, ap); }
static int _vswprintf_p(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return _vswprintf_p_l(b, n, f, NULL, ap); }
static int _swprintf_p_l(wchar_t *b, size_t n, const wchar_t *f, _locale_t l, ...) { va_list ap; int r; va_start(ap, l); r = _vswprintf_p_l(b, n, f, l, ap); va_end(ap); return r; }
static int _swprintf_p(wchar_t *b, size_t n, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = _vswprintf_p_l(b, n, f, NULL, ap); va_end(ap); return r; }
#endif

static const char * volatile f1 = "%2$d %1$d";
static const char * volatile f2 = "%1$f";
static const wchar_t * volatile fw = L"%2$d %1$d";
static const char * volatile cname = "C";

TC_CASE(null_locale) { char a[32], c[32]; _sprintf_p(a, 32, f1, 1, 2); _sprintf_p_l(c, 32, f1, NULL, 1, 2); CHECK_STR(a, "2 1"); CHECK_STR(c, a); }
TC_CASE(explicit_c)
{
    char b[32]; int n; _locale_t l = _create_locale(LC_ALL, cname);
    CHECK(l != NULL); if (!l) return;
    n = _sprintf_p_l(b, 32, f1, l, 1, 2); CHECK_STR(b, "2 1"); CHECK_EQ_I(n, 3);
    _free_locale(l);
}
TC_CASE(wide_null_locale) { wchar_t b[32]; int n = _swprintf_p_l(b, 32, fw, NULL, 1, 2); CHECK_WSTR(b, L"2 1"); CHECK_EQ_I(n, 3); }
TC_CASE(decimal_point)
{
    char b[32]; double v = 1.5; _locale_t l = _create_locale(LC_ALL, cname);
    CHECK(l != NULL); if (!l) return;
    _sprintf_p_l(b, 32, f2, l, v); CHECK_STR(b, "1.500000");
    _free_locale(l);
}

int main(void) { tc_begin(); TC_RUN(null_locale); TC_RUN(explicit_c); TC_RUN(wide_null_locale); TC_RUN(decimal_point); return tc_end(); }

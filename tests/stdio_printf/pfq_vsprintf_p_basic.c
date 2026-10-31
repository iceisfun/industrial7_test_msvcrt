/* TC
 * id:    pfq_vsprintf_p_basic
 * area:  stdio_printf
 * funcs: _vsprintf_p _vsprintf_p_l _vswprintf_p _vswprintf_p_l
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p positional parameters; _sprintf_p, _sprintf_p_l, _swprintf_p
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
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

static int vs(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = _vsprintf_p(b, n, f, ap); va_end(ap); return r; }
static int vsl(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = _vsprintf_p_l(b, n, f, NULL, ap); va_end(ap); return r; }
static int vw(wchar_t *b, size_t n, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = _vswprintf_p(b, n, f, ap); va_end(ap); return r; }
static int vwl(wchar_t *b, size_t n, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = _vswprintf_p_l(b, n, f, NULL, ap); va_end(ap); return r; }
static int twice(char *b1, char *b2, const char *f, ...)
{
    va_list ap, ap2; int r;
    va_start(ap, f); va_copy(ap2, ap);
    r = _vsprintf_p(b1, 32, f, ap); r += _vsprintf_p(b2, 32, f, ap2);
    va_end(ap2); va_end(ap); return r;
}

static const char * volatile f_a = "%2$s %1$d";
static const char * volatile f_r = "%1$d+%1$d";
static const wchar_t * volatile fw = L"%2$ls %1$d";

TC_CASE(narrow) { char b[32]; int n = vs(b, sizeof b, f_a, 5, "x"); CHECK_STR(b, "x 5"); CHECK_EQ_I(n, 3); }
TC_CASE(wide) { wchar_t b[32]; int n = vw(b, 32, fw, 5, L"x"); CHECK_WSTR(b, L"x 5"); CHECK_EQ_I(n, 3); }
TC_CASE(reuse) { char b[32]; vs(b, sizeof b, f_r, 8); CHECK_STR(b, "8+8"); }
TC_CASE(l_null_locale) { char a[32], c[32]; int n1 = vs(a, 32, f_a, 5, "x"), n2 = vsl(c, 32, f_a, 5, "x"); CHECK_EQ_I(n1, n2); CHECK_STR(a, c); }
TC_CASE(wide_l_null_locale) { wchar_t a[32], c[32]; int n1 = vw(a, 32, fw, 5, L"x"), n2 = vwl(c, 32, fw, 5, L"x"); CHECK_EQ_I(n1, n2); CHECK_WSTR(a, c); CHECK_WSTR(c, L"x 5"); }
TC_CASE(va_copy_twice) { char a[32], c[32]; int n = twice(a, c, f_a, 6, "y"); CHECK_STR(a, "y 6"); CHECK_STR(c, "y 6"); CHECK_EQ_I(n, 6); }

int main(void) { tc_begin(); TC_RUN(narrow); TC_RUN(wide); TC_RUN(reuse); TC_RUN(l_null_locale); TC_RUN(wide_l_null_locale); TC_RUN(va_copy_twice); return tc_end(); }

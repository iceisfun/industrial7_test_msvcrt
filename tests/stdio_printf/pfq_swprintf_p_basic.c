/* TC
 * id:    pfq_swprintf_p_basic
 * area:  stdio_printf
 * funcs: _swprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p positional parameters; _sprintf_p, _sprintf_p_l, _swprintf_p
 * note:  Only %ls/%hs asserted (plain %s/%c differ by mode).
 */
#include "tc.h"
#include <stdio.h>
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

static const wchar_t * volatile f1 = L"%2$d %1$d";
static const wchar_t * volatile f2 = L"%2$ls|%1$ls";
static const wchar_t * volatile f3 = L"%1$hs";
static const wchar_t * volatile f4 = L"%1$d %1$d";

TC_CASE(swap) { wchar_t b[32]; int n = _swprintf_p(b, 32, f1, 1, 2); CHECK_WSTR(b, L"2 1"); CHECK_EQ_I(n, 3); }
TC_CASE(wide_strings) { wchar_t b[32]; int n = _swprintf_p(b, 32, f2, L"one", L"two"); CHECK_WSTR(b, L"two|one"); CHECK_EQ_I(n, 7); }
TC_CASE(narrow_hs) { wchar_t b[32]; _swprintf_p(b, 32, f3, "narrow"); CHECK_WSTR(b, L"narrow"); }
TC_CASE(reuse) { wchar_t b[32]; _swprintf_p(b, 32, f4, 9); CHECK_WSTR(b, L"9 9"); }
TC_CASE(count_and_nul)
{
    wchar_t b[8]; int n = _swprintf_p(b, 8, f1, 10, 20), i;
    CHECK_EQ_I(n, 5); CHECK_EQ_I(b[5], 0);
    for (i = 0; i < 5; i++) CHECK(b[i] == L"20 10"[i]);
}

int main(void) { tc_begin(); TC_RUN(swap); TC_RUN(wide_strings); TC_RUN(narrow_hs); TC_RUN(reuse); TC_RUN(count_and_nul); return tc_end(); }

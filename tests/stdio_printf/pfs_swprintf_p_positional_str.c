/* TC
 * id:    pfs_swprintf_p_positional_str
 * area:  stdio_printf
 * funcs: _swprintf_p
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _printf_p, _sprintf_p, _swprintf_p (positional parameters)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#if TC_CRT_VER >= 140
/* ucrt headers define _swprintf_p inline over __stdio_common_vswprintf_p (default _CRT_INTERNAL_PRINTF_LEGACY_WIDE_SPECIFIERS=4: %s is wide); mingw headers omit it. */
static int __cdecl _swprintf_p(wchar_t *buf, size_t cnt, const wchar_t *fmt, ...)
{ int r; va_list ap; va_start(ap, fmt); r = __stdio_common_vswprintf_p(4, buf, cnt, fmt, NULL, ap); va_end(ap); return r < 0 ? -1 : r; }
#endif

static const wchar_t *volatile a = L"one", *volatile b2 = L"two";
static const char *volatile nar = "nar";

TC_CASE(swap) { wchar_t b[64]; int n = _swprintf_p(b, 64, L"%2$s %1$s", a, b2); CHECK_WSTR(b, L"two one"); CHECK_EQ_I(n, 7); }
TC_CASE(narrow_input) { wchar_t b[64]; int n = _swprintf_p(b, 64, L"%1$hs!", nar); CHECK_WSTR(b, L"nar!"); CHECK_EQ_I(n, 4); }
TC_CASE(char_reuse) { wchar_t b[64]; int n = _swprintf_p(b, 64, L"%1$c%1$c", (wint_t)L'k'); CHECK_WSTR(b, L"kk"); CHECK_EQ_I(n, 2); }

int main(void) { tc_begin(); TC_RUN(swap); TC_RUN(narrow_input); TC_RUN(char_reuse); return tc_end(); }

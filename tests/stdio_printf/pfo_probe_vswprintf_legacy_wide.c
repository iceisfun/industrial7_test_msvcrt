/* TC
 * id:    pfo_probe_vswprintf_legacy_wide
 * area:  stdio_printf
 * funcs: __stdio_common_vswprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "Format specification syntax" (_CRT_STDIO_LEGACY_WIDE_SPECIFIERS): %s in wide functions is a wide string in legacy mode
 * note:  PROBE: option 0x4 (_CRT_INTERNAL_PRINTF_LEGACY_WIDE_SPECIFIERS) makes %s / %c in wide printf take wide arguments,
 *        while without it %s is a narrow string. Checked for vswprintf only.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int callw(unsigned long long o, wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(default_s_is_narrow)
{ wchar_t b[32]; int r = callw(0, b, 32, L"%s", "nar"); CHECK_WSTR(b, L"nar"); CHECK_EQ_I(r, 3); }
TC_CASE(legacy_s_is_wide)
{ wchar_t b[32]; int r = callw(4, b, 32, L"%s", L"wid"); CHECK_WSTR(b, L"wid"); CHECK_EQ_I(r, 3); }
TC_CASE(legacy_ls_still_wide)
{ wchar_t b[32]; callw(4, b, 32, L"%ls", L"wid"); CHECK_WSTR(b, L"wid"); }
int main(void) { tc_begin(); TC_RUN(default_s_is_narrow); TC_RUN(legacy_s_is_wide); TC_RUN(legacy_ls_still_wide); return tc_end(); }

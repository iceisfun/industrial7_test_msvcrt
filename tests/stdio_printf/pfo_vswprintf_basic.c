/* TC
 * id:    pfo_vswprintf_basic
 * area:  stdio_printf
 * funcs: __stdio_common_vswprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.29.2.7 swprintf/vswprintf; C11 7.21.6.12 snprintf analogue; MS docs vswprintf, printf type field (ls, c, lc)
 * note:  option 0x2 (_CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR) gives the C99-style return of the would-be length
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>

static int callw(unsigned long long o, wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(mixed)
{
    wchar_t b[64]; int r;
    r = callw(0, b, 64, L"%d|%ls|%c|%lc", -17, L"wide", 'x', (wint_t)L'y');
    CHECK_WSTR(b, L"-17|wide|x|y"); CHECK_EQ_I(r, 12);
}
TC_CASE(trunc_standard)
{
    wchar_t b[8]; volatile size_t n = 4; int r;
    r = callw(2, b, n, L"%ls", L"abcdefgh");
    CHECK_EQ_I(r, 8); CHECK_WSTR(b, L"abc");
}
TC_CASE(count_only)
{
    volatile size_t n = 0; int r;
    r = callw(2, NULL, n, L"%d-%ls", 12345, L"xy");
    CHECK_EQ_I(r, 8);
}
TC_CASE(width_prec)
{
    wchar_t b[64];
    callw(0, b, 64, L"[%6ls][%-6ls][%.2ls][%6.3ls]", L"ab", L"ab", L"abcdef", L"abcdef");
    CHECK_WSTR(b, L"[    ab][ab    ][ab][   abc]");
}
TC_CASE(percent)
{
    wchar_t b[16]; int r = callw(0, b, 16, L"100%%");
    CHECK_WSTR(b, L"100%"); CHECK_EQ_I(r, 4);
}
int main(void)
{ tc_begin(); TC_RUN(mixed); TC_RUN(trunc_standard); TC_RUN(count_only); TC_RUN(width_prec); TC_RUN(percent); return tc_end(); }

/* TC
 * id:    pfu_printf_s_basic
 * area:  stdio_printf
 * funcs: printf_s wprintf_s vprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   MS docs printf_s, wprintf_s, vprintf_s (return number of characters printed)
 * note:  stdout is redirected by the harness; only return values are asserted. Output is tiny.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int vp(const char *fmt, ...) { va_list ap; int r; va_start(ap, fmt); r = vprintf_s(fmt, ap); va_end(ap); return r; }

TC_CASE(empty) { volatile const char *f = ""; CHECK_EQ_I(printf_s((const char *)f), 0); }
TC_CASE(abc_nl) { volatile const char *f = "abc\n"; CHECK_EQ_I(printf_s((const char *)f), 4); }
TC_CASE(with_args) { CHECK_EQ_I(printf_s("%d-%s\n", 12, "x"), 5); }
TC_CASE(vprintf_s_count) { CHECK_EQ_I(vp("%d\n", 123), 4); }
TC_CASE(wide) { volatile const wchar_t *f = L""; CHECK_EQ_I(wprintf_s((const wchar_t *)f), 0); CHECK_EQ_I(wprintf_s(L"ab\n"), 3); }

int main(void)
{
    tc_begin();
    TC_RUN(empty); TC_RUN(abc_nl); TC_RUN(with_args); TC_RUN(vprintf_s_count); TC_RUN(wide);
    fflush(stdout);
    return tc_end();
}

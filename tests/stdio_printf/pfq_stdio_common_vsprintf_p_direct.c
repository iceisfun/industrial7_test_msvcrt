/* TC
 * id:    pfq_stdio_common_vsprintf_p_direct
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf_p __stdio_common_vswprintf_p
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   <stdio.h> / <corecrt_stdio_config.h> in the Windows 10 SDK (inline _sprintf_p_l etc. call these with options = *__local_stdio_printf_options(), initially 0)
 * note:  Internal/undocumented API called directly. Count-only (NULL, 0) is how the SDK implements _vscprintf_p_l.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

static int cp(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_p(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int cpw(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf_p(0, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(reorder) { char b[32]; int r = cp(b, sizeof b, "%2$d %1$d", 1, 2); CHECK_EQ_I(r, 3); CHECK_STR(b, "2 1"); }
TC_CASE(count_only)
{
    int r = cp(NULL, 0, "%2$d %1$d", 1, 2);
    CHECK_EQ_I(r, 3);
    r = cp(NULL, 0, "%1$s%1$s%1$s", "abcd");
    CHECK_EQ_I(r, 12);
}
TC_CASE(wide_reorder)
{
    wchar_t b[32]; int r = cpw(b, 32, L"%2$ls-%1$d", 5, L"ab");
    CHECK_EQ_I(r, 4);
    CHECK_WSTR(b, L"ab-5");
    r = cpw(NULL, 0, L"%2$ls-%1$d", 5, L"ab");
    CHECK_EQ_I(r, 4);
}
TC_CASE(null_locale_uses_current)
{
    char b[32]; int r = cp(b, sizeof b, "%1$.2f", 1.5);
    CHECK_EQ_I(r, 4);
    CHECK_STR(b, "1.50");
}
TC_CASE(exact_fit)
{
    char b[8]; int r = cp(b, 4, "%1$s", "abc");
    CHECK_EQ_I(r, 3);
    CHECK_STR(b, "abc");
}
int main(void)
{
    tc_begin();
    TC_RUN(reorder); TC_RUN(count_only); TC_RUN(wide_reorder); TC_RUN(null_locale_uses_current); TC_RUN(exact_fit);
    return tc_end();
}

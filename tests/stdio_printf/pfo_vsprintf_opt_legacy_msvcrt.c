/* TC
 * id:    pfo_vsprintf_opt_legacy_msvcrt
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  inferred
 * ref:   corecrt_stdio_config.h
 * note:  only the unchanged baseline under options=0x8 for ordinary int/string/float formatting is asserted; effects of 0x8 are in pfo_probe_vsprintf_opt_msvcrt_compat.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int vs(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static int vw(unsigned long long o, wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile int vi = 42;

TC_CASE(int_str)
{
    char b[64]; int n = vs(8, b, sizeof b, "%d|%s|%5.2f", (int)vi, "abc", 3.14159);
    CHECK_STR(b, "42|abc| 3.14"); CHECK_EQ_I(n, 12);
}
TC_CASE(float_basic)
{
    char b[64];
    vs(8, b, sizeof b, "%f|%.3f|%g", 1.5, 2.0, 0.25);
    CHECK_STR(b, "1.500000|2.000|0.25");
}
TC_CASE(equals_opt0)
{
    char a[64], c[64];
    vs(0, a, sizeof a, "%x|%o|%10s|%-4d|", 255, 8, "r", 7);
    vs(8, c, sizeof c, "%x|%o|%10s|%-4d|", 255, 8, "r", 7);
    CHECK_STR(a, c);
}
int main(void)
{
    tc_begin();
    TC_RUN(int_str); TC_RUN(float_basic); TC_RUN(equals_opt0);
    return tc_end();
}

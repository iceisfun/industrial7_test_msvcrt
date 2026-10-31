/* TC
 * id:    pfo_vsprintf_opt_legacy_nullterm
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _snprintf/_vsnprintf return value; corecrt_stdio_config.h _CRT_INTERNAL_PRINTF_LEGACY_VSPRINTF_NULL_TERMINATION (used by the UCRT inline _vsnprintf)
 * note:  options=1: overflow returns negative, and when there is room the string is NUL-terminated and len returned. The NUL-at-buf[n-1] state on overflow/exact fit is NOT asserted here (undocumented); see pfo_probe_vsprintf_legacy_nullterm_exact.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int vs(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile int vi = 42;

TC_CASE(overflow_returns_neg)
{
    char b[16]; int n;
    memset(b, 'Z', sizeof b);
    n = vs(1, b, 4, "abcdefgh");
    CHECK(n < 0);
}
TC_CASE(overflow_prefix_stored)
{
    char b[16];
    memset(b, 'Z', sizeof b);
    vs(1, b, 4, "abcdefgh");
    CHECK_EQ_I(b[0], 'a'); CHECK_EQ_I(b[1], 'b'); CHECK_EQ_I(b[2], 'c'); CHECK_EQ_I(b[4], 'Z');
}
TC_CASE(fits)
{
    char b[16]; int n;
    memset(b, 'Z', sizeof b);
    n = vs(1, b, 9, "abcdefgh");
    CHECK_EQ_I(n, 8); CHECK_STR(b, "abcdefgh");
}
int main(void)
{
    tc_begin();
    TC_RUN(overflow_returns_neg); TC_RUN(overflow_prefix_stored); TC_RUN(fits);
    return tc_end();
}

/* TC
 * id:    pfo_probe_vsprintf_legacy_nullterm_exact
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   corecrt_stdio_config.h
 * note:  PROBE: options=1 (a) on overflow (len > n) whether buf[n-1] becomes NUL, and (b) with len == n (exact fit without room for NUL): return value and whether buf[n-1] is overwritten by NUL. Expected most likely: return len, last byte NUL (legacy NUL termination wins).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int vs(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile int vi = 42;

TC_CASE(overflow_nul_terminated)
{
    char b[16];
    memset(b, 'Z', sizeof b);
    vs(1, b, 4, "abcdefgh");
    CHECK_EQ_I(b[3], 0); CHECK_EQ_I(b[4], 'Z');
}
TC_CASE(exact_return)
{
    char b[16]; int n;
    memset(b, 'Z', sizeof b);
    n = vs(1, b, 8, "abcdefgh");
    CHECK_EQ_I(n, 8);
}
TC_CASE(exact_last_byte)
{
    char b[16];
    memset(b, 'Z', sizeof b);
    vs(1, b, 8, "abcdefgh");
    CHECK_EQ_I(b[7], 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(overflow_nul_terminated); TC_RUN(exact_return); TC_RUN(exact_last_byte);
    return tc_end();
}

/* TC
 * id:    pfo_probe_vsprintf_legacy_nullterm_exact
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  documented
 * ref:   corecrt_stdio_config.h
 * note:  options=1 is the legacy _vsnprintf mode. Observed on Windows 10 22H2 and matching MS docs _snprintf ("If len = count, len characters are stored,
 *        no null terminator is appended, and len is returned"): on overflow (len > n) exactly n bytes are stored and buf[n-1] is the last text byte, NOT NUL;
 *        with len == n the return is len and the last byte is the last text character (no NUL).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int vs(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile int vi = 42;

TC_CASE(overflow_not_nul_terminated)
{
    char b[16];
    memset(b, 'Z', sizeof b);
    vs(1, b, 4, "abcdefgh");
    CHECK_EQ_I(b[3], 'd'); CHECK_EQ_I(b[4], 'Z');
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
    CHECK_EQ_I(b[7], 'h');
}
int main(void)
{
    tc_begin();
    TC_RUN(overflow_not_nul_terminated); TC_RUN(exact_return); TC_RUN(exact_last_byte);
    return tc_end();
}

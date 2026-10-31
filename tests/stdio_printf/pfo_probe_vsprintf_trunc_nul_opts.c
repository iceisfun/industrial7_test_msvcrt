/* TC
 * id:    pfo_probe_vsprintf_trunc_nul_opts
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snprintf, _vsnprintf ("if len == count no NUL is appended"); corecrt_stdio_config.h option bits
 * note:  PROBE: byte at buf[n-1] and beyond after truncation / exact fit for options 0, 1 (LEGACY_VSPRINTF_NULL_TERMINATION)
 *        and 2 (STANDARD_SNPRINTF_BEHAVIOR). Terminator byte at b[n-1] is accepted as either the last char or NUL (not asserted); return values asserted only where docs agree (negative for legacy
 *        truncation, would-be length for 0x2). Complements pfo_probe_vsprintf_trunc_nul.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int call(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Z'; b[15] = 0; }

TC_CASE(opts0_exact_fit)
{   /* options 0 (no legacy flag): only the first n-1 chars and the "nothing beyond n" guard are asserted;
       whether the CRT NUL-terminates by overwriting b[n-1] is unknown */
    char b[16]; volatile int n = 5; int r; fill(b);
    r = call(0, b, n, "%s", "abcde");
    CHECK(r == 5 || r < 0);
    CHECK_MEM(b, "abcd", 4);
    CHECK(b[4] == 'e' || b[4] == 0);
    CHECK_EQ_I(b[5], 'Z');
}
TC_CASE(opts0_truncate)
{
    char b[16]; volatile int n = 3; int r; fill(b);
    r = call(0, b, n, "%s", "abcde");
    CHECK(r < 0);
    CHECK_MEM(b, "ab", 2);
    CHECK(b[2] == 'c' || b[2] == 0);
    CHECK_EQ_I(b[3], 'Z');
}
TC_CASE(opts1_exact_fit)
{   /* probe: legacy _vsnprintf: n == len -> returns len, all n chars stored, nothing written beyond */
    char b[16]; volatile int n = 5; int r; fill(b);
    r = call(1, b, n, "%s", "abcde");
    CHECK_EQ_I(r, 5);
    CHECK_MEM(b, "abcde", 5);
    CHECK_EQ_I(b[5], 'Z');
}
TC_CASE(opts1_truncate)
{   /* docs: -1; whether b[n-1] is 'c' (no NUL) or 0 (CRT terminates) is the probe */
    char b[16]; volatile int n = 3; int r; fill(b);
    r = call(1, b, n, "%s", "abcde");
    CHECK(r < 0);
    CHECK_MEM(b, "ab", 2);
    CHECK(b[2] == 'c' || b[2] == 0);
    CHECK_EQ_I(b[3], 'Z');
}
TC_CASE(opts2_exact_and_trunc)
{
    char b[16]; volatile int n = 5; int r; fill(b);
    r = call(2, b, n, "%s", "abcde");
    CHECK_EQ_I(r, 5);
    CHECK_STR(b, "abcd");
    fill(b); n = 3;
    r = call(2, b, n, "%s", "abcde");
    CHECK_EQ_I(r, 5);
    CHECK_STR(b, "ab");
}
int main(void)
{
    tc_begin();
    TC_RUN(opts0_exact_fit); TC_RUN(opts0_truncate); TC_RUN(opts1_exact_fit); TC_RUN(opts1_truncate);
    TC_RUN(opts2_exact_and_trunc);
    return tc_end();
}

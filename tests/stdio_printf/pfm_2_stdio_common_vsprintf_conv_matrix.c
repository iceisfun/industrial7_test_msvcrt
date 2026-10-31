/* TC
 * id:    pfm_2_stdio_common_vsprintf_conv_matrix
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   >=140
 * tier:  p0
 * conf:  observed
 * ref:   Windows SDK corecrt_stdio_config.h / stdio.h (declared, otherwise undocumented); C11 7.21.6.1 for the conversions
 * note:  Direct calls with locale NULL. options 0 is what sprintf passes; options 2 (_CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR)
 *        is what the C99 snprintf/vsnprintf inlines pass: always null-terminates and returns the would-be length.
 *        Option bit 1 (LEGACY_VSPRINTF_NULL_TERMINATION) behaviour at an exact fit is not asserted, only recorded with TC NOTE lines.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int sv(unsigned __int64 opt, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(opt, b, n, f, NULL, ap); va_end(ap); return r; }
static void note(const char *k, long long v) { char b[32]; tc_write("TC NOTE " TC_TEST_ID " "); tc_write(k); tc_write("="); tc_i64(v, b); tc_write(b); tc_write("\n"); }

static volatile int vm5 = -5, v255 = 255;
static volatile unsigned vbig = 4000000000u;
static volatile double v15 = 1.5, v1 = 1.0;
static const char *volatile longs = "abcdefg";

TC_CASE(opt0_ints)  { char b[64]; int r = sv(0, b, 64, "%i|%u|%X", vm5, vbig, v255); CHECK_STR(b, "-5|4000000000|FF"); CHECK_EQ_I(r, 16); }
TC_CASE(opt0_F)     { char b[64]; int r = sv(0, b, 64, "%F|%.2F", v15, v15); CHECK_STR(b, "1.500000|1.50"); CHECK_EQ_I(r, 13); }
TC_CASE(opt0_a)     { char b[64]; int r = sv(0, b, 64, "%.2a|%.1A", v1, v1); CHECK_STR(b, "0x1.00p+0|0X1.0P+0"); CHECK_EQ_I(r, 18); }
TC_CASE(opt0_e)     { char b[64]; int r = sv(0, b, 64, "%e", v15); CHECK_STR(b, "1.500000e+00"); CHECK_EQ_I(r, 12); }
TC_CASE(opt2_truncation_returns_full)
{
    char b[8]; int r; b[7] = 'Z';
    r = sv(2, b, 4, "%s", longs);
    CHECK_EQ_I(r, 7); CHECK_STR(b, "abc"); CHECK_EQ_I(b[7], 'Z');
}
TC_CASE(opt2_count_zero)
{
    char b[4] = "zzz"; int r = sv(2, b, 0, "%i-%X", vm5, v255);
    CHECK_EQ_I(r, 5); CHECK_STR(b, "zzz");
}
TC_CASE(opt2_exact_fit_terminates)
{
    char b[8]; int r; b[4] = 'Z';
    r = sv(2, b, 4, "%s", "abc");
    CHECK_EQ_I(r, 3); CHECK_STR(b, "abc");
}
TC_CASE(opt1_exact_fit_recorded)
{
    char b[8]; int r, i; for (i = 0; i < 8; i++) b[i] = 'Z';
    r = sv(1, b, 4, "%s", "abcd");
    CHECK(b[0] == 'a' && b[1] == 'b' && b[2] == 'c');
    note("opt1_exact_ret", r); note("opt1_exact_b3", b[3]);
}
int main(void)
{
    tc_begin();
    TC_RUN(opt0_ints); TC_RUN(opt0_F); TC_RUN(opt0_a); TC_RUN(opt0_e);
    TC_RUN(opt2_truncation_returns_full); TC_RUN(opt2_count_zero); TC_RUN(opt2_exact_fit_terminates); TC_RUN(opt1_exact_fit_recorded);
    return tc_end();
}

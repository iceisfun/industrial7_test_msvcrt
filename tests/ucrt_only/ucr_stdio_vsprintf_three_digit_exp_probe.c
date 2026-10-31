/* TC
 * id:    ucr_stdio_vsprintf_three_digit_exp_probe
 * area:  ucrt_only
 * funcs: __stdio_common_vsprintf
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_output_format / _TWO_DIGIT_EXPONENT; corecrt_stdio_config.h _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS (0x10)
 * note:  PROBE: option bit 0x10 of __stdio_common_vsprintf makes %e print three-digit exponents (e+005). Option bit value taken from corecrt_stdio_config.h. Default two-digit case is separate.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

static int vs(unsigned long long opts, char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsprintf(opts, b, n, f, NULL, ap); va_end(ap);
    return r;
}
static double mk(unsigned long long bits) { union { double d; unsigned long long u; } x; x.u = bits; return x.d; }
static volatile unsigned long long v_bits;
static double dv(unsigned long long bits) { v_bits = bits; return mk(v_bits); }

TC_CASE(default_two_digit) { char b[64]; vs(0, b, sizeof b, "%e", 1e5); CHECK_STR(b, "1.000000e+05"); }
TC_CASE(opt16_small) { char b[64]; vs(0x10, b, sizeof b, "%e", 1e5); CHECK_STR(b, "1.000000e+005"); }
TC_CASE(opt16_three_already) { char b[64]; vs(0x10, b, sizeof b, "%e", 1e100); CHECK_STR(b, "1.000000e+100"); }
TC_CASE(opt16_neg_exp) { char b[64]; vs(0x10, b, sizeof b, "%e", 1e-5); CHECK_STR(b, "1.000000e-005"); }

int main(void)
{
    tc_begin();
    TC_RUN(default_two_digit);
    TC_RUN(opt16_small);
    TC_RUN(opt16_three_already);
    TC_RUN(opt16_neg_exp);
    return tc_end();
}

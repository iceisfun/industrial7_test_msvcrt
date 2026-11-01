/* TC
 * id:    pff_f_round_ties_ucrt
 * area:  stdio_printf
 * funcs: sprintf __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   C11 7.21.6.1 (exact decimal value, round-half-even in the default rounding mode)
 * note:  Observed on Windows 10 22H2 (19045) ucrtbase, probe run in the reference: tie rounding is selected by the caller's options, not by the
 *        build. __stdio_common_vsprintf(options 0x22 = STANDARD_SNPRINTF_BEHAVIOR|STANDARD_ROUNDING) rounds exact ties to even (0.5->0, 2.5->2,
 *        0.125->0.12, -2.5->-2), which is what an MSVC-built sprintf passes. The mingw sprintf used here passes options=2 (no 0x20), so ucrtbase
 *        applies legacy half-away-from-zero (0.5->1, 2.5->3, 0.125->0.13, -2.5->-3). ucrtbase does NOT deviate from ISO C; the sprintf cases
 *        assert the toolchain/options dependent legacy result (class F), the std_* cases assert half-even.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define TF(fmt, val, want) do { char b[1100]; volatile double tc_v = (val); int n = sprintf(b, fmt, tc_v); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

static int sv(unsigned o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
#define TS(fmt, val, want) do { char b[64]; volatile double tc_v = (val); int n = sv(0x22, b, sizeof b, fmt, tc_v); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

TC_CASE(prec0) { TF("%.0f", 0.5, "1"); TF("%.0f", 1.5, "2"); TF("%.0f", 2.5, "3"); TF("%.0f", 3.5, "4"); }
TC_CASE(prec2) { TF("%.2f", 0.125, "0.13"); TF("%.2f", 0.375, "0.38"); TF("%.2f", 0.625, "0.63"); }
TC_CASE(neg) { TF("%.0f", -2.5, "-3"); TF("%.0f", -0.5, "-1"); }
TC_CASE(std_prec0) { TS("%.0f", 0.5, "0"); TS("%.0f", 1.5, "2"); TS("%.0f", 2.5, "2"); TS("%.0f", 3.5, "4"); }
TC_CASE(std_prec2) { TS("%.2f", 0.125, "0.12"); TS("%.2f", 0.375, "0.38"); TS("%.2f", 0.625, "0.62"); }
TC_CASE(std_neg) { TS("%.0f", -2.5, "-2"); TS("%.0f", -0.5, "-0"); }

int main(void)
{
    tc_begin();
    TC_RUN(prec0); TC_RUN(prec2); TC_RUN(neg);
    TC_RUN(std_prec0); TC_RUN(std_prec2); TC_RUN(std_neg);
    return tc_end();
}

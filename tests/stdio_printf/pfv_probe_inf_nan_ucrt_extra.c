/* TC
 * id:    pfv_probe_inf_nan_ucrt_extra
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win10
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf 'Infinity and NaN' formatting
 * note:  PROBES: %F/%E/%G inf -> INF, -nan(ind) -> -NAN(IND); SNaN 0x7ff4.. -> nan(snan); negative -> -nan(snan); %08f inf -> '     inf' (0 flag ignored); %.2f inf -> inf; %+f inf -> +inf, '% f' inf -> ' inf' (sign flags on inf).
 */
#include "tc.h"
#include <stdio.h>
static double D(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static volatile unsigned long long b_pinf = 0x7ff0000000000000ULL, b_ninf = 0xfff0000000000000ULL, b_qnan = 0x7ff8000000000000ULL, b_ind = 0xfff8000000000000ULL, b_snan = 0x7ff4000000000000ULL, b_nsnan = 0xfff4000000000000ULL;
#define PINF D(b_pinf)
#define NINF D(b_ninf)
#define QNAN D(b_qnan)
#define IND D(b_ind)
#define SNAN D(b_snan)

TC_CASE(upper_E_F_G) { char b[64]; sprintf(b, "%F|%E|%G", PINF, PINF, PINF); CHECK_STR(b, "INF|INF|INF"); sprintf(b, "%F", IND); CHECK_STR(b, "-NAN(IND)"); }
TC_CASE(snan) { char b[64]; sprintf(b, "%f", SNAN); CHECK_STR(b, "nan(snan)"); }
TC_CASE(neg_snan) { char b[64]; sprintf(b, "%f", D(b_nsnan)); CHECK_STR(b, "-nan(snan)"); }
TC_CASE(zero_flag_inf) { char b[64]; sprintf(b, "%08f", PINF); CHECK_STR(b, "     inf"); }
TC_CASE(precision_inf) { char b[64]; sprintf(b, "%.2f", PINF); CHECK_STR(b, "inf"); }
TC_CASE(plus_space_flags) { char b[64]; sprintf(b, "%+f", PINF); CHECK_STR(b, "+inf"); sprintf(b, "% f", PINF); CHECK_STR(b, " inf"); }
int main(void) { tc_begin(); TC_RUN(upper_E_F_G); TC_RUN(snan); TC_RUN(neg_snan); TC_RUN(zero_flag_inf); TC_RUN(precision_inf); TC_RUN(plus_space_flags); return tc_end(); }

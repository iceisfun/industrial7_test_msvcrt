/* TC
 * id:    pfv_inf_nan_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win10
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs printf 'Infinity and NaN' formatting
 * note:  Counterpart of pfv_inf_nan_pre140.
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

TC_CASE(inf_f) { char b[64]; sprintf(b, "%f", PINF); CHECK_STR(b, "inf"); }
TC_CASE(neg_inf) { char b[64]; sprintf(b, "%f", NINF); CHECK_STR(b, "-inf"); }
TC_CASE(inf_e_g) { char b[64]; sprintf(b, "%e", PINF); CHECK_STR(b, "inf"); sprintf(b, "%g", PINF); CHECK_STR(b, "inf"); sprintf(b, "%g", NINF); CHECK_STR(b, "-inf"); }
TC_CASE(qnan) { char b[64]; sprintf(b, "%f", QNAN); CHECK_STR(b, "nan"); }
TC_CASE(neg_indefinite) { char b[64]; sprintf(b, "%f", IND); CHECK_STR(b, "-nan(ind)"); }
TC_CASE(width_pad) { char b[64]; sprintf(b, "%6f", PINF); CHECK_STR(b, "   inf"); sprintf(b, "%-6f|", PINF); CHECK_STR(b, "inf   |"); }
int main(void) { tc_begin(); TC_RUN(inf_f); TC_RUN(neg_inf); TC_RUN(inf_e_g); TC_RUN(qnan); TC_RUN(neg_indefinite); TC_RUN(width_pad); return tc_end(); }

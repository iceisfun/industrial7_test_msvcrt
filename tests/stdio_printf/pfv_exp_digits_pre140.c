/* TC
 * id:    pfv_exp_digits_pre140
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _set_output_format (default prints three-digit exponents; _TWO_DIGIT_EXPONENT changes it)
 * note:  Counterpart of pfv_exp_digits_ucrt.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
static double D(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static volatile double v1 = 1.0, v12345 = 12345.0, v1em5 = 1e-5, v1e6 = 1e6, v5 = 5.0, v0 = 0.0;
TC_CASE(e_one) { char b[64]; sprintf(b, "%e", v1); CHECK_STR(b, "1.000000e+000"); }
TC_CASE(E_upper) { char b[64]; sprintf(b, "%E", v12345); CHECK_STR(b, "1.234500E+004"); }
TC_CASE(g_small_large) { char b[64]; sprintf(b, "%g", v1em5); CHECK_STR(b, "1e-005"); sprintf(b, "%g", v1e6); CHECK_STR(b, "1e+006"); }
TC_CASE(e_prec0) { char b[64]; sprintf(b, "%.0e", v5); CHECK_STR(b, "5e+000"); }
TC_CASE(zero) { char b[64]; sprintf(b, "%e", v0); CHECK_STR(b, "0.000000e+000"); }
int main(void) { tc_begin(); TC_RUN(e_one); TC_RUN(E_upper); TC_RUN(g_small_large); TC_RUN(e_prec0); TC_RUN(zero); return tc_end(); }

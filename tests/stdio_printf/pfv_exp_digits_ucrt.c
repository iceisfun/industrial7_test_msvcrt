/* TC
 * id:    pfv_exp_digits_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win10
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1 (exponent at least two digits); MS docs printf
 * note:  Counterpart of pfv_exp_digits_pre140.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
static double D(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static volatile double v1 = 1.0, v12345 = 12345.0, v1em5 = 1e-5, v1e6 = 1e6, v1e5 = 100000.0, v5 = 5.0, v0 = 0.0, v1e100 = 1e100;
TC_CASE(e_one) { char b[64]; sprintf(b, "%e", v1); CHECK_STR(b, "1.000000e+00"); }
TC_CASE(E_upper) { char b[64]; sprintf(b, "%E", v12345); CHECK_STR(b, "1.234500E+04"); }
TC_CASE(g_small_large) { char b[64]; sprintf(b, "%g", v1em5); CHECK_STR(b, "1e-05"); sprintf(b, "%g", v1e6); CHECK_STR(b, "1e+06"); sprintf(b, "%g", v1e5); CHECK_STR(b, "100000"); }
TC_CASE(e_prec0) { char b[64]; sprintf(b, "%.0e", v5); CHECK_STR(b, "5e+00"); }
TC_CASE(zero) { char b[64]; sprintf(b, "%e", v0); CHECK_STR(b, "0.000000e+00"); }
TC_CASE(exp_100) { char b[64]; sprintf(b, "%e", v1e100); CHECK_STR(b, "1.000000e+100"); }
int main(void) { tc_begin(); TC_RUN(e_one); TC_RUN(E_upper); TC_RUN(g_small_large); TC_RUN(e_prec0); TC_RUN(zero); TC_RUN(exp_100); return tc_end(); }

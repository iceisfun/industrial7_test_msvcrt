/* TC
 * id:    pfm_1_len_l_float
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p7 (l has no effect on e/f/g conversions); MS docs printf; _set_output_format (pre-UCRT three-digit exponents)
 * note:  Pre-UCRT runtimes print three exponent digits by default (e+000); UCRT prints two. F/a/A are in pfm_1_len_l_float_faa_probe.
 */
#include "tc.h"
#include <stdio.h>

#if TC_CRT_VER >= 140
#define E0 "e+00"
#define EM5 "e-05"
#define UE0 "E+00"
#define UEM5 "E-05"
#else
#define E0 "e+000"
#define EM5 "e-005"
#define UE0 "E+000"
#define UEM5 "E-005"
#endif
static char b[64];
TC_CASE(lf) { volatile double d = 1.5; sprintf(b, "%lf", d); CHECK_STR(b, "1.500000"); }
TC_CASE(le) { volatile double d = 1.5; sprintf(b, "%le", d); CHECK_STR(b, "1.500000" E0); }
TC_CASE(lE) { volatile double d = 1.5; sprintf(b, "%lE", d); CHECK_STR(b, "1.500000" UE0); }
TC_CASE(lg) { volatile double d = 1.5; sprintf(b, "%lg", d); CHECK_STR(b, "1.5"); }
TC_CASE(lG) { volatile double d = 1e-5; sprintf(b, "%lG", d); CHECK_STR(b, "1" UEM5); }
TC_CASE(lg_small) { volatile double d = 1e-5; sprintf(b, "%lg", d); CHECK_STR(b, "1" EM5); }

int main(void)
{
    tc_begin();
    TC_RUN(lf); TC_RUN(le); TC_RUN(lE); TC_RUN(lg); TC_RUN(lG); TC_RUN(lg_small);
    return tc_end();
}

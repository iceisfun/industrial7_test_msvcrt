/* TC
 * id:    pfu_sprintf_s_exp_digits
 * area:  stdio_printf
 * funcs: sprintf_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  documented
 * ref:   MS docs printf format specification (e/E/g: exponent has at least two digits); _set_output_format (three digits only in legacy mode)
 * note:  UCRT default is at least two exponent digits. Legacy CRTs (msvcr90..120, msvcrt) are tested in pfu_sprintf_s_exp_digits_legacy_probe.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(e_two_digits)  { char b[64]; volatile double d = 1e5; int n = sprintf_s(b, sizeof b, "%e", d); CHECK_STR(b, "1.000000e+05"); CHECK_EQ_I(n, 12); }
TC_CASE(e_negative_exp) { char b[64]; volatile double d = 1e-5; sprintf_s(b, sizeof b, "%e|%E", d, d); CHECK_STR(b, "1.000000e-05|1.000000E-05"); }
TC_CASE(e_three_digits_needed) { char b[64]; volatile double d = 1e100; sprintf_s(b, sizeof b, "%e", d); CHECK_STR(b, "1.000000e+100"); }
TC_CASE(g_exponent) { char b[64]; volatile double d = 1e-5, e = 1e20; sprintf_s(b, sizeof b, "%g|%g", d, e); CHECK_STR(b, "1e-05|1e+20"); }

int main(void) { tc_begin(); TC_RUN(e_two_digits); TC_RUN(e_negative_exp); TC_RUN(e_three_digits_needed); TC_RUN(g_exponent); return tc_end(); }

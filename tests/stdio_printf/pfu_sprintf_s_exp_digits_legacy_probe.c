/* TC
 * id:    pfu_sprintf_s_exp_digits_legacy_probe
 * area:  stdio_printf
 * funcs: sprintf_s
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _set_output_format (legacy default: three-digit exponent unless _TWO_DIGIT_EXPONENT is set)
 * note:  Probing exactly: pre-UCRT default exponent width in %e/%g is THREE digits ("e+005"). 1e100 has three digits regardless.
 *        msvcrt.dll deliberately omitted (unsure).
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(e_three_digits) { char b[64]; volatile double d = 1e5; sprintf_s(b, sizeof b, "%e", d); CHECK_STR(b, "1.000000e+005"); }
TC_CASE(g_three_digits) { char b[64]; volatile double d = 1e-5; sprintf_s(b, sizeof b, "%g", d); CHECK_STR(b, "1e-005"); }
TC_CASE(e_1e100) { char b[64]; volatile double d = 1e100; sprintf_s(b, sizeof b, "%e", d); CHECK_STR(b, "1.000000e+100"); }

int main(void) { tc_begin(); TC_RUN(e_three_digits); TC_RUN(g_three_digits); TC_RUN(e_1e100); return tc_end(); }

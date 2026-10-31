/* TC
 * id:    pff_g_exp_legacy
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win7
 * tier:  p1
 * conf:  observed
 * ref:   MS docs printf type field (legacy CRTs print three exponent digits, _set_output_format era)
 * note:  Pre-UCRT CRTs pad exponents to three digits; msvcr120 and msvcr110 included.
 */
#include "tc.h"
#include <stdio.h>

static volatile double a = 1e-5, b10 = 1e10, c = 1e100, d = 1e-10, e = 123456789.0;

TC_CASE(neg5) { char b[64]; sprintf(b, "%g", a); CHECK_STR(b, "1e-005"); }
TC_CASE(pos10) { char b[64]; sprintf(b, "%g", b10); CHECK_STR(b, "1e+010"); }
TC_CASE(pos100) { char b[64]; sprintf(b, "%g", c); CHECK_STR(b, "1e+100"); }
TC_CASE(upper) { char b[64]; sprintf(b, "%G", d); CHECK_STR(b, "1E-010"); }
TC_CASE(digits) { char b[64]; sprintf(b, "%g", e); CHECK_STR(b, "1.23457e+008"); }

int main(void) { tc_begin(); TC_RUN(neg5); TC_RUN(pos10); TC_RUN(pos100); TC_RUN(upper); TC_RUN(digits); return tc_end(); }

/* TC
 * id:    pff_g_exp_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf type field g/G; UCRT prints at least two exponent digits
 */
#include "tc.h"
#include <stdio.h>

static volatile double a = 1e-5, b10 = 1e10, c = 1e100, d = 1e-10, e = 123456789.0;

TC_CASE(neg5) { char b[64]; sprintf(b, "%g", a); CHECK_STR(b, "1e-05"); }
TC_CASE(pos10) { char b[64]; sprintf(b, "%g", b10); CHECK_STR(b, "1e+10"); }
TC_CASE(pos100) { char b[64]; sprintf(b, "%g", c); CHECK_STR(b, "1e+100"); }
TC_CASE(upper) { char b[64]; sprintf(b, "%G", d); CHECK_STR(b, "1E-10"); }
TC_CASE(digits) { char b[64]; sprintf(b, "%g", e); CHECK_STR(b, "1.23457e+08"); }

int main(void) { tc_begin(); TC_RUN(neg5); TC_RUN(pos10); TC_RUN(pos100); TC_RUN(upper); TC_RUN(digits); return tc_end(); }

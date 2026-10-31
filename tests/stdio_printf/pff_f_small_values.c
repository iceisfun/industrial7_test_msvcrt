/* TC
 * id:    pff_f_small_values
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1 f conversion
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile double v_1e10 = 1e-10, v_n1e10 = -1e-10, v_1e300 = 1e-300, v_den = 4.9406564584124654e-324;
static char b[128];

TC_CASE(six) { sprintf(b, "%f", v_1e10); CHECK_STR(b, "0.000000"); }
TC_CASE(ten) { sprintf(b, "%.10f", v_1e10); CHECK_STR(b, "0.0000000001"); }
TC_CASE(neg) { sprintf(b, "%f", v_n1e10); CHECK_STR(b, "-0.000000"); }
TC_CASE(tiny) { sprintf(b, "%.5f", v_1e300); CHECK_STR(b, "0.00000"); }
TC_CASE(denormal) { CHECK(v_den > 0.0); sprintf(b, "%f", v_den); CHECK_STR(b, "0.000000"); }
int main(void) { tc_begin(); TC_RUN(six); TC_RUN(ten); TC_RUN(neg); TC_RUN(tiny); TC_RUN(denormal); return tc_end(); }

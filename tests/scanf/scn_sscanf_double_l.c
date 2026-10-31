/* TC
 * id:    scn_sscanf_double_l
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p12; MS docs scanf type field (l prefix = double)
 * note:  0.1 bit-exact asserted only for ucrtbase (correct rounding); older CRTs use CHECK_NEAR.
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

static double rd(const char *in, const char *fmt) { double d = -123.0; sscanf(in, fmt, &d); return d; }
TC_CASE(point_one) {
    double d = rd("0.1", "%lf");
#ifdef TC_UCRT
    CHECK_BITS_D(d, 0x1.999999999999ap-4);
#endif
    CHECK_NEAR(d, 0.1, 1e-15);
}
TC_CASE(big) { double d = rd("1e308", "%le"); CHECK(d > 9.99e307 && d < 1.01e308); }
TC_CASE(exact_frac) { CHECK_BITS_D(rd("123456789.125", "%lg"), 123456789.125); }
TC_CASE(dot_forms) { CHECK_BITS_D(rd(".5", "%lf"), 0.5); CHECK_BITS_D(rd("5.", "%lf"), 5.0); }
TC_CASE(plus_exp) { CHECK_BITS_D(rd("+1.5E+2", "%lf"), 150.0); }

int main(void) { tc_begin(); TC_RUN(point_one); TC_RUN(big); TC_RUN(exact_frac); TC_RUN(dot_forms); TC_RUN(plus_exp); return tc_end(); }

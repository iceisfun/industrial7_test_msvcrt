/* TC
 * id:    scn_sscanf_float_width
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.2p9 (field width), MS docs scanf width specification
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_CASE(w3) { double d = 0, e = 0; int n = sscanf("1.2345", "%3lf%lf", &d, &e); CHECK_EQ_I(n, 2); CHECK_NEAR(d, 1.2, 1e-15); CHECK_BITS_D(e, 345.0); }
TC_CASE(w2) { double d = 0, e = 0; int n = sscanf("12.5", "%2lf%lf", &d, &e); CHECK_EQ_I(n, 2); CHECK_BITS_D(d, 12.0); CHECK_BITS_D(e, 0.5); }
TC_CASE(w5_split) { double d = 0, e = 0; int n = sscanf("1.2345678", "%5lf%lf", &d, &e); CHECK_EQ_I(n, 2); CHECK_NEAR(d, 1.234, 1e-12); CHECK_BITS_D(e, 5678.0); }
TC_CASE(sign_counts) { double d = 0, e = 0; int n = sscanf("-1.25", "%3lf%lf", &d, &e); CHECK_EQ_I(n, 2); CHECK_BITS_D(d, -1.0); CHECK_BITS_D(e, 25.0); }
TC_CASE(width_float_target) { float f = 0; int n = sscanf("7.89", "%3f", &f); CHECK_EQ_I(n, 1); CHECK_NEAR(f, 7.8, 1e-6); }

int main(void) { tc_begin(); TC_RUN(w3); TC_RUN(w2); TC_RUN(w5_split); TC_RUN(sign_counts); TC_RUN(width_float_target); return tc_end(); }

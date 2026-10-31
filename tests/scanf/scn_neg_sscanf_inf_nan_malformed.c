/* TC
 * id:    scn_neg_sscanf_inf_nan_malformed
 * area:  scanf
 * funcs: sscanf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.21.6.2 matching failure
 * note:  Malformed inf/nan/float text: matching failure, return 0, destination untouched.
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;

TC_CASE(inx) { double d = 5.0; int r; v = "inx"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 0); CHECK_BITS_D(d, 5.0); }
TC_CASE(nax) { double d = 5.0; int r; v = "nax"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 0); CHECK_BITS_D(d, 5.0); }
TC_CASE(sign_only) { double d = 5.0; int r; v = "-x"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 0); CHECK_BITS_D(d, 5.0); }
TC_CASE(dot_only) { double d = 5.0; int r; v = ".x"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 0); CHECK_BITS_D(d, 5.0); }
TC_CASE(exp_only) { double d = 5.0; int r; v = "e5"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 0); CHECK_BITS_D(d, 5.0); }
TC_CASE(second_fails) { double a = 0, b = 5.0; int r; v = "2.5 inx"; r = sscanf((const char *)v, "%lf %lf", &a, &b); CHECK_EQ_I(r, 1); CHECK_BITS_D(a, 2.5); CHECK_BITS_D(b, 5.0); }
int main(void) { tc_begin(); TC_RUN(inx); TC_RUN(nax); TC_RUN(sign_only); TC_RUN(dot_only); TC_RUN(exp_only); TC_RUN(second_fails); return tc_end(); }

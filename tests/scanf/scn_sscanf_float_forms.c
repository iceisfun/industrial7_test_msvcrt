/* TC
 * id:    scn_sscanf_float_forms
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.2, 7.22.1.3; MS docs scanf type field characters
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static const char *volatile v_fmt;

TC_CASE(leading_dot) { double d = -1; int n = sscanf(".5", "%lf", &d); CHECK_EQ_I(n, 1); CHECK_BITS_D(d, 0.5); }
TC_CASE(trailing_dot) { double d = -1; int n = sscanf("5.", "%lf", &d); CHECK_EQ_I(n, 1); CHECK_BITS_D(d, 5.0); }
TC_CASE(neg_frac_exp) { double d = 0; int n = sscanf("-.5e1", "%lf", &d); CHECK_EQ_I(n, 1); CHECK_BITS_D(d, -5.0); }
TC_CASE(plus_exp) { double d = 0, e = 0; int n = sscanf("1e+2 2E-1", "%lf %lf", &d, &e); CHECK_EQ_I(n, 2); CHECK_BITS_D(d, 100.0); CHECK_NEAR(e, 0.2, 1e-15); }
TC_CASE(exp_then_char) { double d = 0; char c = 0; int n = sscanf("1E2x", "%lf%c", &d, &c); CHECK_EQ_I(n, 2); CHECK_BITS_D(d, 100.0); CHECK_EQ_I(c, 'x'); }
TC_CASE(second_dot) { double d = 0; char c = 0; int n = sscanf("1.5.5", "%lf%c", &d, &c); CHECK_EQ_I(n, 2); CHECK_BITS_D(d, 1.5); CHECK_EQ_I(c, '.'); }
TC_CASE(exp_digits) { double d = 0; int cnt = -1, n = sscanf("2e3", "%lf%n", &d, &cnt); CHECK_EQ_I(n, 1); CHECK_BITS_D(d, 2000.0); CHECK_EQ_I(cnt, 3); }
TC_CASE(float_target) { float f = 0; int n = sscanf("0.25", "%f", &f); CHECK_EQ_I(n, 1); CHECK_BITS_F(f, 0.25f); }

int main(void) { tc_begin(); (void)v_fmt; TC_RUN(leading_dot); TC_RUN(trailing_dot); TC_RUN(neg_frac_exp); TC_RUN(plus_exp); TC_RUN(exp_then_char); TC_RUN(second_dot); TC_RUN(exp_digits); TC_RUN(float_target); return tc_end(); }

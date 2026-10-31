/* TC
 * id:    scn_sscanf_float_basic
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p12 (a,e,f,g conversions)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

static float rd(const char *in, const char *fmt) { float f = -123.0f; sscanf(in, fmt, &f); return f; }
TC_CASE(f_3_5) { CHECK_BITS_F(rd("3.5", "%f"), 3.5f); }
TC_CASE(neg) { CHECK_BITS_F(rd("-0.25", "%f"), -0.25f); }
TC_CASE(exp) { CHECK_BITS_F(rd("1e3", "%f"), 1000.0f); }
TC_CASE(exp_neg) { CHECK_NEAR(rd("1.5E-2", "%f"), 0.015, 1e-8); }
TC_CASE(e_g_same) { CHECK_BITS_F(rd("2.5e1", "%e"), 25.0f); CHECK_BITS_F(rd("2.5e1", "%g"), 25.0f); CHECK_BITS_F(rd("-0.75", "%e"), -0.75f); CHECK_BITS_F(rd("-0.75", "%g"), -0.75f); }

int main(void) { tc_begin(); TC_RUN(f_3_5); TC_RUN(neg); TC_RUN(exp); TC_RUN(exp_neg); TC_RUN(e_g_same); return tc_end(); }

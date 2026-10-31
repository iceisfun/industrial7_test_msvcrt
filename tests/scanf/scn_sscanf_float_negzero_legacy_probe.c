/* TC
 * id:    scn_sscanf_float_negzero_legacy_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   win7
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * note:  PROBE: pre-UCRT CRTs (msvcrt, msvcr80-120) preserve the sign of a parsed negative zero (IEEE/C99 strtod semantics); not documented for those versions.
 * ref:   C11 7.22.1.3p4 (strtod sign applies to zero), F.3; MS docs scanf
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_CASE(neg_zero_dbl) { double d = 1; int n = sscanf("-0.0", "%lf", &d); CHECK_EQ_I(n, 1); CHECK_BITS_D(d, -0.0); CHECK(tc_dbits(d) == 0x8000000000000000ull); }
TC_CASE(pos_zero_exp) { double d = 1; int n = sscanf("0e0", "%lf", &d); CHECK_EQ_I(n, 1); CHECK(tc_dbits(d) == 0ull); }
TC_CASE(neg_zero_float) { float f = 1; int n = sscanf("-0", "%f", &f); CHECK_EQ_I(n, 1); CHECK_BITS_F(f, -0.0f); CHECK(tc_fbits(f) == 0x80000000u); }
TC_CASE(neg_zero_exp) { double d = 1; int n = sscanf("-0e5", "%lf", &d); CHECK_EQ_I(n, 1); CHECK_BITS_D(d, -0.0); }

int main(void) { tc_begin(); TC_RUN(neg_zero_dbl); TC_RUN(pos_zero_exp); TC_RUN(neg_zero_float); TC_RUN(neg_zero_exp); return tc_end(); }

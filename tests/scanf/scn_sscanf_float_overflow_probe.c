/* TC
 * id:    scn_sscanf_float_overflow_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.6.2p10 (undefined if not representable); MS docs scanf
 * note:  PROBE: out-of-range float text. Expected: return value 1 and +-HUGE_VAL (inf bits) on overflow, +0.0 on underflow; real CRTs are believed to behave like strtod.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_CASE(pos_overflow) { double d = 0; int n = sscanf("1e999", "%lf", &d); CHECK_EQ_I(n, 1); CHECK(tc_dbits(d) == 0x7ff0000000000000ull); }
TC_CASE(neg_overflow) { double d = 0; int n = sscanf("-1e999", "%lf", &d); CHECK_EQ_I(n, 1); CHECK(tc_dbits(d) == 0xfff0000000000000ull); }
TC_CASE(underflow) { double d = 1; int n = sscanf("1e-999", "%lf", &d); CHECK_EQ_I(n, 1); CHECK(tc_dbits(d) == 0ull); }
TC_CASE(float_overflow) { float f = 0; int n = sscanf("1e50", "%f", &f); CHECK_EQ_I(n, 1); CHECK(tc_fbits(f) == 0x7f800000u); }

int main(void) { tc_begin(); TC_RUN(pos_overflow); TC_RUN(neg_overflow); TC_RUN(underflow); TC_RUN(float_overflow); return tc_end(); }

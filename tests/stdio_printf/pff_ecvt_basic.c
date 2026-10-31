/* TC
 * id:    pff_ecvt_basic
 * area:  stdio_printf
 * funcs: _ecvt
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _ecvt, _fcvt (count = total significant digits)
 */
#include "tc.h"
#include <stdlib.h>

#define T(val, cnt, str, dec, sgn) do { volatile double tc_v = (val); int d = -99, s = -99; char *r = _ecvt(tc_v, cnt, &d, &s); \
    CHECK_STR(r, str); CHECK_EQ_I(d, dec); CHECK_EQ_I(s, sgn); } while (0)
TC_CASE(pi)     { T(3.14159, 4, "3142", 1, 0); }
TC_CASE(big)    { T(12345.0, 3, "123", 5, 0); }
TC_CASE(small)  { T(0.00123, 2, "12", -2, 0); }
TC_CASE(neg)    { T(-5.0, 2, "50", 1, 1); }
TC_CASE(carry)  { T(9.99, 2, "10", 2, 0); }
int main(void) { tc_begin(); TC_RUN(pi); TC_RUN(big); TC_RUN(small); TC_RUN(neg); TC_RUN(carry); return tc_end(); }

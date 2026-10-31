/* TC
 * id:    cv_ecvt_fcvt_rounding
 * area:  conv
 * funcs: _ecvt _fcvt
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _ecvt, _fcvt (digit string, decimal-point position, sign)
 * note:  Only non-tie roundings and carries here; exact ties are in cv_ecvt_fcvt_tie_probe.
 */
#include "tc.h"
#include <stdlib.h>

static volatile double v;

TC_CASE(ecvt_carry) { int d = -9, s = -9; v = 9.9999; CHECK_STR(_ecvt(v, 3, &d, &s), "100"); CHECK_EQ_I(d, 2); CHECK_EQ_I(s, 0); }
TC_CASE(fcvt_carry) { int d = -9, s = -9; v = 0.999; CHECK_STR(_fcvt(v, 2, &d, &s), "100"); CHECK_EQ_I(d, 1); CHECK_EQ_I(s, 0); }
TC_CASE(ecvt_one) { int d = -9, s = -9; v = 1.0; CHECK_STR(_ecvt(v, 1, &d, &s), "1"); CHECK_EQ_I(d, 1); CHECK_EQ_I(s, 0); }
TC_CASE(fcvt_round_up) { int d = -9, s = -9; v = 1.26; CHECK_STR(_fcvt(v, 1, &d, &s), "13"); CHECK_EQ_I(d, 1); CHECK_EQ_I(s, 0); }
TC_CASE(neg_sign) { int d = -9, s = -9; v = -9.9999; CHECK_STR(_ecvt(v, 3, &d, &s), "100"); CHECK_EQ_I(d, 2); CHECK_EQ_I(s, 1); }

int main(void) { tc_begin(); TC_RUN(ecvt_carry); TC_RUN(fcvt_carry); TC_RUN(ecvt_one); TC_RUN(fcvt_round_up); TC_RUN(neg_sign); return tc_end(); }

/* TC
 * id:    cv_ecvt_fcvt_tie_probe
 * area:  conv
 * funcs: _fcvt
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _fcvt
 * note:  PROBE: exactly representable ties round half away from zero in _fcvt: _fcvt(0.5,0) -> "1" decpt 1; _fcvt(2.5,0) -> "3" decpt 1.
 *        Some UCRT versions might round half-even instead ("" / "2").
 */
#include "tc.h"
#include <stdlib.h>

static volatile double v;
TC_CASE(half) { int d = -9, s = -9; v = 0.5; CHECK_STR(_fcvt(v, 0, &d, &s), "1"); CHECK_EQ_I(d, 1); CHECK_EQ_I(s, 0); }
TC_CASE(two_half) { int d = -9, s = -9; v = 2.5; CHECK_STR(_fcvt(v, 0, &d, &s), "3"); CHECK_EQ_I(d, 1); CHECK_EQ_I(s, 0); }

int main(void) { tc_begin(); TC_RUN(half); TC_RUN(two_half); return tc_end(); }

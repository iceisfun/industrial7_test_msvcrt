/* TC
 * id:    gsm_ecvt_fcvt_edge_probe
 * area:  string
 * funcs: _ecvt _fcvt
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _ecvt/_fcvt (sign is nonzero if value is negative)
 * note:  Probes: (1) _ecvt(-0.0,4) reports sign != 0 (sign bit), digits "0000"; (2) _ecvt(123.456, 0) returns an empty string;
 *        (3) _fcvt(0.0, 3) -> "000" with decpt 0; (4) _ecvt(0.0, 4) -> "0000" with decpt 0 (some libcs, e.g. glibc, use 1).
 *        Separate cases per guess.
 */
#include "tc.h"
#include <stdlib.h>
static volatile double v;
TC_CASE(neg_zero_sign) { int d = -9, s = 0; v = -0.0; (void)_ecvt(v, 4, &d, &s); CHECK(s != 0); }
TC_CASE(neg_zero_digits) { int d = -9, s = 0; char *r; v = -0.0; r = _ecvt(v, 4, &d, &s); CHECK_STR(r, "0000"); CHECK_EQ_I(d, 0); }
TC_CASE(ecvt_zero) { int d = -9, s = -9; char *r; v = 0.0; r = _ecvt(v, 4, &d, &s); CHECK_STR(r, "0000"); CHECK_EQ_I(d, 0); CHECK_EQ_I(s, 0); }
TC_CASE(count_zero) { int d = -9, s = -9; char *r; v = 123.456; r = _ecvt(v, 0, &d, &s); CHECK_STR(r, ""); }
TC_CASE(fcvt_zero) { int d = -9, s = -9; char *r; v = 0.0; r = _fcvt(v, 3, &d, &s); CHECK_STR(r, "000"); CHECK_EQ_I(d, 0); CHECK_EQ_I(s, 0); }
int main(void) { tc_begin(); TC_RUN(neg_zero_sign); TC_RUN(neg_zero_digits); TC_RUN(ecvt_zero); TC_RUN(count_zero); TC_RUN(fcvt_zero); return tc_end(); }

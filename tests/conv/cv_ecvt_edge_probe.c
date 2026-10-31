/* TC
 * id:    cv_ecvt_edge_probe
 * area:  conv
 * funcs: _ecvt
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _ecvt
 * note:  Probes: _ecvt with count 0, only (exact ties are in cv_ecvt_fcvt_tie_probe). Each in its own case.
 */
#include "tc.h"
#include <stdlib.h>

static volatile double v_d;

TC_CASE(count0_empty) { int dec = -99, sign = -99; char *r; v_d = 123.456; r = _ecvt(v_d, 0, &dec, &sign); CHECK_STR(r, ""); }
TC_CASE(count0_dec) { int dec = -99, sign = -99; v_d = 123.456; _ecvt(v_d, 0, &dec, &sign); CHECK_EQ_I(dec, 3); }
TC_CASE(zero_dec) { int dec = -99, sign = -99; v_d = 0.0; _ecvt(v_d, 5, &dec, &sign); CHECK_EQ_I(dec, 0); }

int main(void)
{
    tc_begin();
    TC_RUN(count0_empty); TC_RUN(count0_dec); TC_RUN(zero_dec);
    return tc_end();
}

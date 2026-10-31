/* TC
 * id:    cv_fcvt_edge_probe
 * area:  conv
 * funcs: _fcvt
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _fcvt
 * note:  Probes: negative count (rounding left of the point) and value rounding to zero digits.
 */
#include "tc.h"
#include <stdlib.h>

static volatile double v_d;

TC_CASE(negative_count) { int dec = -99, sign = -99; char *r; v_d = 1234.5; r = _fcvt(v_d, -2, &dec, &sign); CHECK_STR(r, "12"); CHECK_EQ_I(dec, 4); }
TC_CASE(rounds_to_nothing) { int dec = -99, sign = -99; char *r; v_d = 0.0005; r = _fcvt(v_d, 2, &dec, &sign); CHECK_STR(r, ""); CHECK_EQ_I(dec, -2); }

int main(void)
{
    tc_begin();
    TC_RUN(negative_count); TC_RUN(rounds_to_nothing);
    return tc_end();
}

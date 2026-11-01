/* TC
 * id:    cv_fcvt_edge_probe
 * area:  conv
 * funcs: _fcvt
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _fcvt
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice): negative_count _fcvt(1234.5,-2) -> "12" dec 4 (as guessed);
 *        _fcvt(0.0005, 2) -> "" with decpt -3 (the original probe guessed -2; the value is 0.5e-3, so decpt -3 is the consistent answer).
 *        Unverified on pre-UCRT CRTs (kept the old guess there).
 */
#include "tc.h"
#include <stdlib.h>

#if TC_AT_LEAST(140)
#define EXP_DEC -3   /* observed on Win10 22H2 ucrtbase: 0.0005 = 0.5e-3 -> decpt -3 */
#else
#define EXP_DEC -2   /* unverified guess for pre-UCRT CRTs */
#endif
static volatile double v_d;

TC_CASE(negative_count) { int dec = -99, sign = -99; char *r; v_d = 1234.5; r = _fcvt(v_d, -2, &dec, &sign); CHECK_STR(r, "12"); CHECK_EQ_I(dec, 4); }
TC_CASE(rounds_to_nothing) { int dec = -99, sign = -99; char *r; v_d = 0.0005; r = _fcvt(v_d, 2, &dec, &sign); CHECK_STR(r, ""); CHECK_EQ_I(dec, EXP_DEC); }

int main(void)
{
    tc_begin();
    TC_RUN(negative_count); TC_RUN(rounds_to_nothing);
    return tc_end();
}

/* TC
 * id:    cv_fcvt_basic
 * area:  conv
 * funcs: _fcvt
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _fcvt
 */
#include "tc.h"
#include <stdlib.h>

static volatile double v_d;

TC_CASE(basic) { int dec = -99, sign = -99; char *r; v_d = 3.14159; r = _fcvt(v_d, 2, &dec, &sign); CHECK_STR(r, "314"); CHECK_EQ_I(dec, 1); CHECK_EQ_I(sign, 0); }
TC_CASE(negative) { int dec = -99, sign = -99; char *r; v_d = -3.14159; r = _fcvt(v_d, 2, &dec, &sign); CHECK_STR(r, "314"); CHECK_EQ_I(dec, 1); CHECK_EQ_I(sign, 1); }
TC_CASE(count0) { int dec = -99, sign = -99; char *r; v_d = 3.14159; r = _fcvt(v_d, 0, &dec, &sign); CHECK_STR(r, "3"); CHECK_EQ_I(dec, 1); }
TC_CASE(large) { int dec = -99, sign = -99; char *r; v_d = 1e15; r = _fcvt(v_d, 2, &dec, &sign); CHECK_STR(r, "100000000000000000"); CHECK_EQ_I(dec, 16); }
TC_CASE(more_digits) { int dec = -99, sign = -99; char *r; v_d = 1.5; r = _fcvt(v_d, 3, &dec, &sign); CHECK_STR(r, "1500"); CHECK_EQ_I(dec, 1); }

int main(void)
{
    tc_begin();
    TC_RUN(basic); TC_RUN(negative); TC_RUN(count0); TC_RUN(large); TC_RUN(more_digits);
    return tc_end();
}

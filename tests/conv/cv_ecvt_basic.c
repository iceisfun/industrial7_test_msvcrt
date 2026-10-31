/* TC
 * id:    cv_ecvt_basic
 * area:  conv
 * funcs: _ecvt
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _ecvt
 */
#include "tc.h"
#include <stdlib.h>

static volatile double v_d;

TC_CASE(basic) { int dec = -99, sign = -99; char *r; v_d = 123.456; r = _ecvt(v_d, 5, &dec, &sign); CHECK_STR(r, "12346"); CHECK_EQ_I(dec, 3); CHECK_EQ_I(sign, 0); }
TC_CASE(negative) { int dec = -99, sign = -99; char *r; v_d = -123.456; r = _ecvt(v_d, 5, &dec, &sign); CHECK_STR(r, "12346"); CHECK_EQ_I(dec, 3); CHECK_EQ_I(sign, 1); }
TC_CASE(zero) { int dec = -99, sign = -99; char *r; v_d = 0.0; r = _ecvt(v_d, 5, &dec, &sign); CHECK_STR(r, "00000"); CHECK_EQ_I(sign, 0); }
TC_CASE(small) { int dec = -99, sign = -99; char *r; v_d = 0.00123; r = _ecvt(v_d, 5, &dec, &sign); CHECK_STR(r, "12300"); CHECK_EQ_I(dec, -2); }
TC_CASE(large) { int dec = -99, sign = -99; char *r; v_d = 1e10; r = _ecvt(v_d, 5, &dec, &sign); CHECK_STR(r, "10000"); CHECK_EQ_I(dec, 11); }
TC_CASE(static_buffer_reused) { int d1, s1, d2, s2; char *a, *b; v_d = 1.5; a = _ecvt(v_d, 3, &d1, &s1); v_d = 2.5; b = _ecvt(v_d, 3, &d2, &s2); CHECK(a == b); CHECK_STR(b, "250"); }

int main(void)
{
    tc_begin();
    TC_RUN(basic); TC_RUN(negative); TC_RUN(zero); TC_RUN(small); TC_RUN(large); TC_RUN(static_buffer_reused);
    return tc_end();
}

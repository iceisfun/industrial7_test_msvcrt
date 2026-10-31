/* TC
 * id:    pff_fcvt_basic
 * area:  stdio_printf
 * funcs: _fcvt
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _ecvt, _fcvt (digit string without decimal point, decpt, sign)
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(pi)    { volatile double v = 3.14159; int d = -99, s = -99; char *r = _fcvt(v, 2, &d, &s); CHECK_STR(r, "314"); CHECK_EQ_I(d, 1); CHECK_EQ_I(s, 0); }
TC_CASE(neg)   { volatile double v = -123.456; int d = -99, s = -99; char *r = _fcvt(v, 1, &d, &s); CHECK_STR(r, "1235"); CHECK_EQ_I(d, 3); CHECK_EQ_I(s, 1); }
TC_CASE(zero_digits) { volatile double v = 12.7; int d = -99, s = -99; char *r = _fcvt(v, 0, &d, &s); CHECK_STR(r, "13"); CHECK_EQ_I(d, 2); CHECK_EQ_I(s, 0); }
TC_CASE(carry) { volatile double v = 99.96; int d = -99, s = -99; char *r = _fcvt(v, 1, &d, &s); CHECK_STR(r, "1000"); CHECK_EQ_I(d, 3); CHECK_EQ_I(s, 0); }
int main(void) { tc_begin(); TC_RUN(pi); TC_RUN(neg); TC_RUN(zero_digits); TC_RUN(carry); return tc_end(); }

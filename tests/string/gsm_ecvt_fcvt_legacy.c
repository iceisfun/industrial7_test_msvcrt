/* TC
 * id:    gsm_ecvt_fcvt_legacy
 * area:  string
 * funcs: _ecvt _fcvt _gcvt
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _ecvt, _fcvt, _gcvt (_ecvt/_fcvt use a single buffer overwritten by each call; _gcvt writes the caller's buffer)
 * note:  Exponent form of _gcvt differs (e+10 vs e+010) between CRTs and is only probed in gsm_gcvt_exp_digits_probe.
 */
#include "tc.h"
#include <stdlib.h>
static volatile double v;

TC_CASE(ecvt) { int d = -9, s = -9; char *r; v = 123.456; r = _ecvt(v, 5, &d, &s); CHECK_STR(r, "12346"); CHECK_EQ_I(d, 3); CHECK_EQ_I(s, 0); }
TC_CASE(fcvt) { int d = -9, s = -9; char *r; v = 123.456; r = _fcvt(v, 2, &d, &s); CHECK_STR(r, "12346"); CHECK_EQ_I(d, 3); CHECK_EQ_I(s, 0); }
TC_CASE(static_buffer)
{
    int d, s; char *r1, *r2;
    v = 123.456; r1 = _ecvt(v, 5, &d, &s);
    v = 9.0;     r2 = _ecvt(v, 3, &d, &s);
    CHECK_EQ_P(r1, r2); CHECK_STR(r1, "900"); CHECK_EQ_I(d, 1);
}
TC_CASE(gcvt_basic) { char b[32]; char *r; v = 123.456; r = _gcvt(v, 5, b); CHECK_EQ_P(r, b); CHECK_STR(b, "123.46"); }
TC_CASE(gcvt_small) { char b[32]; v = 0.0001; _gcvt(v, 3, b); CHECK_STR(b, "0.0001"); }
TC_CASE(gcvt_neg) { char b[32]; v = -2.5; _gcvt(v, 3, b); CHECK_STR(b, "-2.5"); }
int main(void) { tc_begin(); TC_RUN(ecvt); TC_RUN(fcvt); TC_RUN(static_buffer); TC_RUN(gcvt_basic); TC_RUN(gcvt_small); TC_RUN(gcvt_neg); return tc_end(); }

/* TC
 * id:    gsm_ecvt_fcvt_basic
 * area:  string
 * funcs: _ecvt _fcvt
 * procs: _ecvt_s _fcvt_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _ecvt, _fcvt, _ecvt_s, _fcvt_s (digit string, decimal-point position, sign flag)
 * note:  msvcr80/msvcrt covered by gsm_ecvt_fcvt_legacy. Edge values (zero, -0.0, count 0) are in gsm_ecvt_fcvt_edge_probe.
 */
#include "tc.h"
#include <stdlib.h>

TC_DECL_PROC(int, _ecvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _fcvt_s, (char *, size_t, double, int, int *, int *));
static volatile double v;

TC_CASE(ecvt_pos) { int d = -9, s = -9; char *r; v = 123.456; r = _ecvt(v, 5, &d, &s); CHECK_STR(r, "12346"); CHECK_EQ_I(d, 3); CHECK_EQ_I(s, 0); }
TC_CASE(ecvt_neg_small) { int d = -9, s = 0; char *r; v = -0.00123; r = _ecvt(v, 3, &d, &s); CHECK_STR(r, "123"); CHECK_EQ_I(d, -2); CHECK(s != 0); }
TC_CASE(fcvt_pos) { int d = -9, s = -9; char *r; v = 123.456; r = _fcvt(v, 2, &d, &s); CHECK_STR(r, "12346"); CHECK_EQ_I(d, 3); CHECK_EQ_I(s, 0); }
TC_CASE(ecvt_s_digits)
{
    char b[32]; int d = -9, s = -9;
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    v = 123.456; CHECK_EQ_I(tc_p__ecvt_s(b, sizeof b, v, 5, &d, &s), 0); CHECK_STR(b, "12346"); CHECK_EQ_I(d, 3); CHECK_EQ_I(s, 0);
    v = -0.00123; CHECK_EQ_I(tc_p__ecvt_s(b, sizeof b, v, 3, &d, &s), 0); CHECK_STR(b, "123"); CHECK_EQ_I(d, -2); CHECK(s != 0);
}
TC_CASE(fcvt_s_digits)
{
    char b[32]; int d = -9, s = -9;
    if (!tc_p__fcvt_s) TC_SKIP("not exported");
    v = 123.456; CHECK_EQ_I(tc_p__fcvt_s(b, sizeof b, v, 2, &d, &s), 0); CHECK_STR(b, "12346"); CHECK_EQ_I(d, 3); CHECK_EQ_I(s, 0);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_ecvt_s); TC_LOAD_PROC(_fcvt_s);
    TC_RUN(ecvt_pos); TC_RUN(ecvt_neg_small); TC_RUN(fcvt_pos); TC_RUN(ecvt_s_digits); TC_RUN(fcvt_s_digits);
    return tc_end();
}

/* TC
 * id:    cv_ecvt_s_basic
 * area:  conv
 * funcs:
 * procs: _ecvt_s _fcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _ecvt_s, _fcvt_s (digit string without decimal point; *dec = decimal point position; *sign nonzero if negative)
 * note:  Zero, count==0 and the exact count+1 buffer size are probed separately in cv_ecvt_s_edge_probe.
 */
#include "tc.h"

TC_DECL_PROC(int, _ecvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _fcvt_s, (char *, size_t, double, int, int *, int *));

TC_CASE(ecvt_positive)
{
    char b[32]; int d = -99, s = -99; volatile double v = 123.456;
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__ecvt_s(b, sizeof b, v, 5, &d, &s), 0);
    CHECK_STR(b, "12346"); CHECK_EQ_I(d, 3); CHECK_EQ_I(s, 0);
}
TC_CASE(ecvt_negative)
{
    char b[32]; int d = -99, s = -99; volatile double v = -123.456;
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__ecvt_s(b, sizeof b, v, 5, &d, &s), 0);
    CHECK_STR(b, "12346"); CHECK_EQ_I(d, 3); CHECK_EQ_I(s != 0, 1);
}
TC_CASE(ecvt_small_value)
{
    char b[32]; int d = -99, s = -99; volatile double v = 0.00125;
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__ecvt_s(b, sizeof b, v, 3, &d, &s), 0);
    CHECK_STR(b, "125"); CHECK_EQ_I(d, -2); CHECK_EQ_I(s, 0);
}
TC_CASE(fcvt_positive)
{
    char b[32]; int d = -99, s = -99; volatile double v = 3.14159;
    if (!tc_p__fcvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__fcvt_s(b, sizeof b, v, 2, &d, &s), 0);
    CHECK_STR(b, "314"); CHECK_EQ_I(d, 1); CHECK_EQ_I(s, 0);
}
TC_CASE(fcvt_negative)
{
    char b[32]; int d = -99, s = -99; volatile double v = -3.14159;
    if (!tc_p__fcvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__fcvt_s(b, sizeof b, v, 2, &d, &s), 0);
    CHECK_STR(b, "314"); CHECK_EQ_I(d, 1); CHECK_EQ_I(s != 0, 1);
}
TC_CASE(fcvt_count_zero)
{
    char b[32]; int d = -99, s = -99; volatile double v = 123.456;
    if (!tc_p__fcvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__fcvt_s(b, sizeof b, v, 0, &d, &s), 0);
    CHECK_STR(b, "123"); CHECK_EQ_I(d, 3);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_ecvt_s); TC_LOAD_PROC(_fcvt_s);
    TC_RUN(ecvt_positive); TC_RUN(ecvt_negative); TC_RUN(ecvt_small_value);
    TC_RUN(fcvt_positive); TC_RUN(fcvt_negative); TC_RUN(fcvt_count_zero);
    return tc_end();
}

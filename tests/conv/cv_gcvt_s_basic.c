/* TC
 * id:    cv_gcvt_s_basic
 * area:  conv
 * funcs:
 * procs: _gcvt_s strtod
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _gcvt_s (like %g with the given significant digits)
 * note:  Avoids the historical trailing-dot quirk (e.g. "100.") and exponent-width differences by choosing plain values and round-tripping the rest. Denormals are in cv_gcvt_strtod_denorm_probe (pre-UCRT strtod denormal handling is not documented).
 */
#include "tc.h"

TC_DECL_PROC(int, _gcvt_s, (char *, size_t, double, int));
TC_DECL_PROC(double, strtod, (const char *, char **));

TC_CASE(simple)
{
    char b[32]; volatile double v = 3.14159;
    if (!tc_p__gcvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__gcvt_s(b, sizeof b, v, 5), 0);
    CHECK_STR(b, "3.1416");
}
TC_CASE(negative)
{
    char b[32]; volatile double v = -3.14159;
    if (!tc_p__gcvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__gcvt_s(b, sizeof b, v, 5), 0);
    CHECK_STR(b, "-3.1416");
}
TC_CASE(exact_fraction)
{
    char b[32]; volatile double v = 0.5;
    if (!tc_p__gcvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__gcvt_s(b, sizeof b, v, 10), 0);
    CHECK_STR(b, "0.5");
}
TC_CASE(roundtrip17)
{
    char b[40]; volatile double v1 = 1.0 / 3.0, v2 = 0.1, v3 = -1.7976931348623157e308;
    double r;
    if (!tc_p__gcvt_s || !tc_p_strtod) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__gcvt_s(b, sizeof b, v1, 17), 0); r = tc_p_strtod(b, NULL); CHECK_BITS_D(r, v1);
    CHECK_EQ_I(tc_p__gcvt_s(b, sizeof b, v2, 17), 0); r = tc_p_strtod(b, NULL); CHECK_BITS_D(r, v2);
    CHECK_EQ_I(tc_p__gcvt_s(b, sizeof b, v3, 17), 0); r = tc_p_strtod(b, NULL); CHECK_BITS_D(r, v3);
}

int main(void)
{
    tc_begin(); TC_LOAD_PROC(_gcvt_s); TC_LOAD_PROC(strtod);
    TC_RUN(simple); TC_RUN(negative); TC_RUN(exact_fraction); TC_RUN(roundtrip17);
    return tc_end();
}

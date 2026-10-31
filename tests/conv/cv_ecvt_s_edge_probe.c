/* TC
 * id:    cv_ecvt_s_edge_probe
 * area:  conv
 * funcs:
 * procs: _ecvt_s _fcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _ecvt_s, _fcvt_s
 * note:  Probes (1) zero input: _ecvt_s(0.0,5) -> "00000" dec=0 sign=0; _fcvt_s(0.0,2) -> "000" dec=1;
 *        (2) _ecvt_s(123.456, count 0) -> "" with dec=3; (3) sizeInBytes == count+1 suffices (digits + NUL). Each likely but not documented.
 */
#include "tc.h"

TC_DECL_PROC(int, _ecvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _fcvt_s, (char *, size_t, double, int, int *, int *));

TC_CASE(ecvt_zero)
{
    char b[32]; int d = -99, s = -99; volatile double v = 0.0;
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__ecvt_s(b, sizeof b, v, 5, &d, &s), 0);
    CHECK_STR(b, "00000"); CHECK_EQ_I(d, 0); CHECK_EQ_I(s, 0);
}
TC_CASE(fcvt_zero)
{
    char b[32]; int d = -99, s = -99; volatile double v = 0.0;
    if (!tc_p__fcvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__fcvt_s(b, sizeof b, v, 2, &d, &s), 0);
    CHECK_STR(b, "000"); CHECK_EQ_I(d, 1); CHECK_EQ_I(s, 0);
}
TC_CASE(ecvt_count_zero)
{
    char b[32]; int d = -99, s = -99; volatile double v = 123.456;
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__ecvt_s(b, sizeof b, v, 0, &d, &s), 0);
    CHECK_STR(b, ""); CHECK_EQ_I(d, 3);
}
TC_CASE(ecvt_exact_buffer)
{
    char b[6]; int d = -99, s = -99; volatile double v = 123.456;   /* count + 1 bytes */
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__ecvt_s(b, 6, v, 5, &d, &s), 0);
    CHECK_STR(b, "12346"); CHECK_EQ_I(d, 3);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_ecvt_s); TC_LOAD_PROC(_fcvt_s);
    TC_RUN(ecvt_zero); TC_RUN(fcvt_zero); TC_RUN(ecvt_count_zero); TC_RUN(ecvt_exact_buffer);
    return tc_end();
}

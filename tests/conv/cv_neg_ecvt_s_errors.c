/* TC
 * id:    cv_neg_ecvt_s_errors
 * area:  conv
 * funcs:
 * procs: _ecvt_s _fcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _ecvt_s, _fcvt_s: NULL buffer / NULL dec / NULL sign / sizeInBytes==0 -> EINVAL via invalid parameter handler
 * note:  Undersized-buffer and negative-count codes are in cv_neg_cvt_range_probe.
 */
#include "tc.h"

TC_DECL_PROC(int, _ecvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _fcvt_s, (char *, size_t, double, int, int *, int *));
#define EINVAL_ 22

TC_CASE(ecvt_null_buffer)
{
    int d, s; int n0 = tc_iph_count;
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__ecvt_s(NULL, 10, 1.5, 3, &d, &s), EINVAL_);
    CHECK_EQ_I(tc_iph_count - n0, 1);
}
TC_CASE(ecvt_size_zero)
{
    char b[8] = "zzz"; int d, s; int n0 = tc_iph_count;
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__ecvt_s(b, 0, 1.5, 3, &d, &s), EINVAL_);
    CHECK_EQ_I(tc_iph_count - n0, 1);
    CHECK_STR(b, "zzz");
}
TC_CASE(ecvt_null_dec)
{
    char b[16]; int s; int n0 = tc_iph_count;
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__ecvt_s(b, sizeof b, 1.5, 3, NULL, &s), EINVAL_);
    CHECK_EQ_I(tc_iph_count - n0, 1);
}
TC_CASE(ecvt_null_sign)
{
    char b[16]; int d; int n0 = tc_iph_count;
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__ecvt_s(b, sizeof b, 1.5, 3, &d, NULL), EINVAL_);
    CHECK_EQ_I(tc_iph_count - n0, 1);
}
TC_CASE(fcvt_null_buffer)
{
    int d, s; int n0 = tc_iph_count;
    if (!tc_p__fcvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__fcvt_s(NULL, 10, 1.5, 3, &d, &s), EINVAL_);
    CHECK_EQ_I(tc_iph_count - n0, 1);
}
TC_CASE(fcvt_size_zero)
{
    char b[8]; int d, s; int n0 = tc_iph_count;
    if (!tc_p__fcvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__fcvt_s(b, 0, 1.5, 3, &d, &s), EINVAL_);
    CHECK_EQ_I(tc_iph_count - n0, 1);
}
TC_CASE(fcvt_null_dec_sign)
{
    char b[16]; int d, s; int n0 = tc_iph_count;
    if (!tc_p__fcvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__fcvt_s(b, sizeof b, 1.5, 3, NULL, &s), EINVAL_);
    CHECK_EQ_I(tc_p__fcvt_s(b, sizeof b, 1.5, 3, &d, NULL), EINVAL_);
    CHECK_EQ_I(tc_iph_count - n0, 2);
}

int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_LOAD_PROC(_ecvt_s); TC_LOAD_PROC(_fcvt_s);
    TC_RUN(ecvt_null_buffer); TC_RUN(ecvt_size_zero); TC_RUN(ecvt_null_dec); TC_RUN(ecvt_null_sign);
    TC_RUN(fcvt_null_buffer); TC_RUN(fcvt_size_zero); TC_RUN(fcvt_null_dec_sign);
    return tc_end();
}

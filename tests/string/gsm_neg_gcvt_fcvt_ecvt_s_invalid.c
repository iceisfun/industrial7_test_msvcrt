/* TC
 * id:    gsm_neg_gcvt_fcvt_ecvt_s_invalid
 * area:  string
 * kind:  negative
 * funcs:
 * procs: _gcvt_s _ecvt_s _fcvt_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _gcvt_s, _ecvt_s, _fcvt_s (EINVAL via invalid parameter handler: NULL buffer, size 0 / <=0, NULL decimal/sign pointer, too-small buffer)
 * note:  Too-small buffer is asserted only as failure + handler (EINVAL vs ERANGE not asserted). Buffer state after failure, negative count and _gcvt_s digits>=size
 *        are probed in gsm_neg_cvt_s_probe. msvcr80 excluded (missing from mingw lib).
 */
#include "tc.h"
#define EINVAL_ 22
TC_DECL_PROC(int, _gcvt_s, (char *, size_t, double, int));
TC_DECL_PROC(int, _ecvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _fcvt_s, (char *, size_t, double, int, int *, int *));

TC_CASE(exported) { CHECK(tc_p__gcvt_s && tc_p__ecvt_s && tc_p__fcvt_s); }
TC_CASE(gcvt_null_zero)
{
    char b[16]; tc_iph_count = 0;
    CHECK_EQ_I(tc_p__gcvt_s(NULL, 16, 1.5, 3), EINVAL_); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(tc_p__gcvt_s(b, 0, 1.5, 3), EINVAL_); CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(ecvt_fcvt_null_buffer)
{
    int d, s; tc_iph_count = 0;
    CHECK_EQ_I(tc_p__ecvt_s(NULL, 16, 1.5, 3, &d, &s), EINVAL_); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(tc_p__fcvt_s(NULL, 16, 1.5, 3, &d, &s), EINVAL_); CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(ecvt_fcvt_size_zero)
{
    char b[16]; int d, s; tc_iph_count = 0;
    CHECK_EQ_I(tc_p__ecvt_s(b, 0, 1.5, 3, &d, &s), EINVAL_); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(tc_p__fcvt_s(b, 0, 1.5, 3, &d, &s), EINVAL_); CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(ecvt_fcvt_null_decpt_sign)
{
    char b[16]; int d, s; tc_iph_count = 0;
    CHECK_EQ_I(tc_p__ecvt_s(b, 16, 1.5, 3, NULL, &s), EINVAL_); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(tc_p__ecvt_s(b, 16, 1.5, 3, &d, NULL), EINVAL_); CHECK_EQ_I(tc_iph_count, 2);
    CHECK_EQ_I(tc_p__fcvt_s(b, 16, 1.5, 3, NULL, &s), EINVAL_); CHECK_EQ_I(tc_iph_count, 3);
    CHECK_EQ_I(tc_p__fcvt_s(b, 16, 1.5, 3, &d, NULL), EINVAL_); CHECK_EQ_I(tc_iph_count, 4);
}
TC_CASE(ecvt_size_too_small)
{
    char b[16]; int d, s; tc_iph_count = 0;
    CHECK(tc_p__ecvt_s(b, 4, 123.456, 6, &d, &s) != 0); CHECK_EQ_I(tc_iph_count, 1);
    CHECK(tc_p__ecvt_s(b, 4, 123.456, 4, &d, &s) != 0); CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(control_ok)
{
    char b[16]; int d = -99, s = -99; tc_iph_count = 0;
    CHECK_EQ_I(tc_p__ecvt_s(b, sizeof b, 123.456, 5, &d, &s), 0); CHECK_STR(b, "12346"); CHECK_EQ_I(d, 3); CHECK_EQ_I(s, 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); if (!tc_iph_install()) return 3;
    TC_LOAD_PROC(_gcvt_s); TC_LOAD_PROC(_ecvt_s); TC_LOAD_PROC(_fcvt_s);
    TC_RUN(exported);
    if (!(tc_p__gcvt_s && tc_p__ecvt_s && tc_p__fcvt_s)) return tc_end();
    TC_RUN(gcvt_null_zero); TC_RUN(ecvt_fcvt_null_buffer); TC_RUN(ecvt_fcvt_size_zero); TC_RUN(ecvt_fcvt_null_decpt_sign);
    TC_RUN(ecvt_size_too_small); TC_RUN(control_ok);
    return tc_end();
}

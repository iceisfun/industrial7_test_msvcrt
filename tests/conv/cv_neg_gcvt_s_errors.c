/* TC
 * id:    cv_neg_gcvt_s_errors
 * area:  conv
 * funcs:
 * procs: _gcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _gcvt_s: NULL buffer or sizeInBytes==0 -> EINVAL, invalid parameter handler invoked
 * note:  digits >= size code is probed in cv_neg_cvt_range_probe.
 */
#include "tc.h"

TC_DECL_PROC(int, _gcvt_s, (char *, size_t, double, int));

TC_CASE(null_buffer)
{
    int n0 = tc_iph_count;
    if (!tc_p__gcvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__gcvt_s(NULL, 20, 1.5, 5), 22);
    CHECK_EQ_I(tc_iph_count - n0, 1);
}
TC_CASE(size_zero)
{
    char b[8] = "zzz"; int n0 = tc_iph_count;
    if (!tc_p__gcvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__gcvt_s(b, 0, 1.5, 5), 22);
    CHECK_EQ_I(tc_iph_count - n0, 1);
    CHECK_STR(b, "zzz");
}
TC_CASE(digits_too_large_fails)
{
    char b[8]; int n0 = tc_iph_count;
    if (!tc_p__gcvt_s) TC_SKIP("not exported");
    CHECK(tc_p__gcvt_s(b, 3, 3.14159, 10) != 0);
    CHECK(tc_iph_count - n0 >= 1);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install()); TC_LOAD_PROC(_gcvt_s);
    TC_RUN(null_buffer); TC_RUN(size_zero); TC_RUN(digits_too_large_fails);
    return tc_end();
}

/* TC
 * id:    cv_neg_cvt_range_probe
 * area:  conv
 * funcs:
 * procs: _ecvt_s _gcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _ecvt_s, _fcvt_s, _gcvt_s
 * note:  Probes the error CODE for undersized buffers and negative counts: _ecvt_s(size<=count) -> ERANGE (34);
 *        _gcvt_s(digits>=size) -> EINVAL (22); _ecvt_s(count<0) -> EINVAL. Only "nonzero return and IPH counted" is
 *        asserted in the first case of each group; the exact errno is the probe.
 */
#include "tc.h"

TC_DECL_PROC(int, _ecvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _gcvt_s, (char *, size_t, double, int));

TC_CASE(ecvt_small_buffer_fails)
{
    char b[4]; int d, s;
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    CHECK(tc_p__ecvt_s(b, 4, 123.456, 5, &d, &s) != 0);
}
TC_CASE(ecvt_size_equals_count_fails)
{
    char b[16]; int d, s;   /* real buffer is large; only the claimed size is small */
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    CHECK(tc_p__ecvt_s(b, 5, 123.456, 5, &d, &s) != 0);   /* needs count+1 */
}
TC_CASE(ecvt_small_buffer_code)
{
    char b[4]; int d, s;
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__ecvt_s(b, 4, 123.456, 5, &d, &s), 34);
}
TC_CASE(ecvt_negative_count_code)
{
    char b[16]; int d, s;
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__ecvt_s(b, sizeof b, 123.456, -1, &d, &s), 22);
}
TC_CASE(gcvt_digits_ge_size_code)
{
    char b[8];
    if (!tc_p__gcvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__gcvt_s(b, 5, 3.14159, 5), 22);
}

int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_LOAD_PROC(_ecvt_s); TC_LOAD_PROC(_gcvt_s);
    TC_RUN(ecvt_small_buffer_fails); TC_RUN(ecvt_size_equals_count_fails); TC_RUN(ecvt_small_buffer_code);
    TC_RUN(ecvt_negative_count_code); TC_RUN(gcvt_digits_ge_size_code);
    return tc_end();
}

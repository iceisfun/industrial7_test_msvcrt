/* TC
 * id:    cv_neg_cvt_range_probe
 * area:  conv
 * funcs:
 * procs: _ecvt_s _gcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _ecvt_s, _fcvt_s, _gcvt_s
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice): _ecvt_s(size<=count+1) -> ERANGE (34) with the invalid-parameter handler called once;
 *        _gcvt_s(digits 5, size 5) -> ERANGE (34), NOT EINVAL (22), handler once, buffer emptied; _ecvt_s(count=-1) is accepted: returns 0 with
 *        an empty string and decpt 3 (original guess EINVAL was wrong). Cases renamed to match. Pre-UCRT CRTs not verified.
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
TC_CASE(ecvt_negative_count_succeeds_empty)
{   /* observed Win10 22H2 ucrtbase: a negative count is NOT rejected; returns 0, empty digit string, decpt 3, no handler call */
    char b[16]; int d = -99, s = -99;
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    b[0] = 'x'; tc_iph_count = 0;
    CHECK_EQ_I(tc_p__ecvt_s(b, sizeof b, 123.456, -1, &d, &s), 0);
    CHECK_STR(b, ""); CHECK_EQ_I(d, 3); CHECK_EQ_I(s, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(gcvt_digits_ge_size_erange)
{
    char b[8];
    if (!tc_p__gcvt_s) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__gcvt_s(b, 5, 3.14159, 5), 34);   /* observed: ERANGE, not EINVAL; handler called once; buffer emptied */
    CHECK_EQ_I(tc_iph_count, 1); CHECK_STR(b, "");
}

int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_LOAD_PROC(_ecvt_s); TC_LOAD_PROC(_gcvt_s);
    TC_RUN(ecvt_small_buffer_fails); TC_RUN(ecvt_size_equals_count_fails); TC_RUN(ecvt_small_buffer_code);
    TC_RUN(ecvt_negative_count_succeeds_empty); TC_RUN(gcvt_digits_ge_size_erange);
    return tc_end();
}

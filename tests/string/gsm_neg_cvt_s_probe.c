/* TC
 * id:    gsm_neg_cvt_s_probe
 * area:  string
 * kind:  negative
 * funcs:
 * procs: _gcvt_s _ecvt_s _fcvt_s
 * crt:   >=90
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _gcvt_s / _ecvt_s / _fcvt_s
 * note:  Probes: (1) _gcvt_s with digits >= size fails (nonzero, handler invoked) and buffer[0]==0 (separate case); (2) _ecvt_s
 *        with count too small a buffer leaves buffer[0]==0; (3) _ecvt_s with negative count: OBSERVED on Win10 22H2 ucrtbase it
 *        returns 0 with no invalid-parameter call (the old EINVAL expectation was wrong); (4) _fcvt_s with negative count is
 *        accepted (returns 0, handler not called).
 */
#include "tc.h"
#define EINVAL_ 22
TC_DECL_PROC(int, _gcvt_s, (char *, size_t, double, int));
TC_DECL_PROC(int, _ecvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _fcvt_s, (char *, size_t, double, int, int *, int *));

TC_CASE(gcvt_digits_ge_size)
{
    char b[8]; int i; for (i = 0; i < 8; i++) b[i] = 'x';
    if (!tc_p__gcvt_s) TC_SKIP("missing"); tc_iph_count = 0;
    CHECK(tc_p__gcvt_s(b, 4, 1.5, 4) != 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(gcvt_digits_ge_size_nulls)
{
    char b[8]; int i; for (i = 0; i < 8; i++) b[i] = 'x';
    if (!tc_p__gcvt_s) TC_SKIP("missing"); tc_iph_count = 0;
    (void)tc_p__gcvt_s(b, 4, 1.5, 4); CHECK_EQ_I(b[0], 0);
}
TC_CASE(ecvt_small_buffer_nulls)
{
    char b[8]; int d, s, i; for (i = 0; i < 8; i++) b[i] = 'x';
    if (!tc_p__ecvt_s) TC_SKIP("missing"); tc_iph_count = 0;
    CHECK(tc_p__ecvt_s(b, 4, 123.456, 6, &d, &s) != 0); CHECK_EQ_I(b[0], 0);
}
TC_CASE(ecvt_negative_count)
{
    char b[16]; int d, s;
    if (!tc_p__ecvt_s) TC_SKIP("missing"); tc_iph_count = 0;
    CHECK_EQ_I(tc_p__ecvt_s(b, sizeof b, 123.456, -1, &d, &s), 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(fcvt_negative_count)
{
    char b[16]; int d, s;
    if (!tc_p__fcvt_s) TC_SKIP("missing"); tc_iph_count = 0;
    CHECK_EQ_I(tc_p__fcvt_s(b, sizeof b, 123.456, -1, &d, &s), 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); if (!tc_iph_install()) return 3;
    TC_LOAD_PROC(_gcvt_s); TC_LOAD_PROC(_ecvt_s); TC_LOAD_PROC(_fcvt_s);
    TC_RUN(gcvt_digits_ge_size); TC_RUN(gcvt_digits_ge_size_nulls); TC_RUN(ecvt_small_buffer_nulls); TC_RUN(ecvt_negative_count); TC_RUN(fcvt_negative_count);
    return tc_end();
}

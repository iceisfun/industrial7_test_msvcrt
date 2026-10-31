/* TC
 * id:    cv_neg_itoa_s_invalid_params
 * area:  conv
 * funcs: _itoa_s _ltoa_s _i64toa_s _ui64toa_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _itoa_s: EINVAL for NULL buffer, size 0, radix outside 2..36 (buffer contents on error not asserted)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static char *volatile v_null; static volatile size_t v_n, v_zero; static volatile int v_radix;

TC_CASE(null_buffer) { int r; tc_iph_count = 0; r = _itoa_s(1, v_null, 10, 10); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size_zero) { char b[4] = "abc"; int r; tc_iph_count = 0; r = _itoa_s(1, b, v_zero, 10); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(radix_1) { char b[16]; int r; v_radix = 1; tc_iph_count = 0; r = _itoa_s(1, b, 10, v_radix); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(radix_37) { char b[16]; int r; v_radix = 37; tc_iph_count = 0; r = _itoa_s(1, b, 10, v_radix); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ltoa_radix_0) { char b[16]; int r; v_radix = 0; tc_iph_count = 0; r = _ltoa_s(1L, b, 10, v_radix); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(i64_null) { int r; tc_iph_count = 0; r = _i64toa_s(1, v_null, 10, 10); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ui64_radix_37) { char b[16]; int r; v_radix = 37; tc_iph_count = 0; r = _ui64toa_s(1, b, 10, v_radix); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(iph_counts_each_call) { char b[16]; v_radix = 99; tc_iph_count = 0; _itoa_s(1, b, 10, v_radix); _ltoa_s(1, b, 10, v_radix); _i64toa_s(1, b, 10, v_radix); _ui64toa_s(1, b, 10, v_radix); CHECK_EQ_I(tc_iph_count, 4); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no invalid parameter handler API\n"); return tc_end(); }
    TC_RUN(null_buffer); TC_RUN(size_zero); TC_RUN(radix_1); TC_RUN(radix_37); TC_RUN(ltoa_radix_0); TC_RUN(i64_null); TC_RUN(ui64_radix_37); TC_RUN(iph_counts_each_call);
    return tc_end();
}

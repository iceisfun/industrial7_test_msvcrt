/* TC
 * id:    sec_neg_i64toa_s_args
 * area:  secure_crt
 * funcs: _i64toa_s _ui64toa_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _i64toa_s (Parameter validation table)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

static volatile long long v_i = 255LL;
static volatile unsigned long long v_u = 255ULL;

TC_CASE(null_buffer_EINVAL) { int r; errno = 0; tc_iph_count = 0; r = _i64toa_s(v_i, NULL, 10, 10); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); r = _ui64toa_s(v_u, NULL, 10, 10); CHECK_EQ_I(r, EINVAL); }
TC_CASE(size_zero_EINVAL) { char b[8] = "q"; int r; errno = 0; r = _i64toa_s(v_i, b, 0, 10); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); r = _ui64toa_s(v_u, b, 0, 10); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(b[0], 'q'); }
TC_CASE(radix_1_EINVAL) { char b[70]; int r; errno = 0; r = _i64toa_s(v_i, b, sizeof b, 1); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); r = _ui64toa_s(v_u, b, sizeof b, 1); CHECK_EQ_I(r, EINVAL); }
TC_CASE(radix_37_EINVAL) { char b[70]; int r; errno = 0; r = _i64toa_s(v_i, b, sizeof b, 37); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); r = _ui64toa_s(v_u, b, sizeof b, 37); CHECK_EQ_I(r, EINVAL); }
TC_CASE(handler_counts)
{
    char b[70]; tc_iph_count = 0;
    _i64toa_s(v_i, NULL, 10, 10); _ui64toa_s(v_u, NULL, 10, 10);
    _i64toa_s(v_i, b, 0, 10); _ui64toa_s(v_u, b, 0, 10);
    _i64toa_s(v_i, b, sizeof b, 1); _ui64toa_s(v_u, b, sizeof b, 37);
    CHECK_EQ_I(tc_iph_count, 6);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(null_buffer_EINVAL); TC_RUN(size_zero_EINVAL); TC_RUN(radix_1_EINVAL); TC_RUN(radix_37_EINVAL); TC_RUN(handler_counts);
    return tc_end();
}

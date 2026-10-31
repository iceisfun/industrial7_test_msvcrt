/* TC
 * id:    sec_neg_ultoa_s_args
 * area:  secure_crt
 * funcs: _ultoa_s _ltoa_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _itoa_s, _ltoa_s, _ultoa_s (Parameter validation table)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

static volatile unsigned long v_ul = 255UL;
static volatile long v_l = 255L;

TC_CASE(ultoa_null_buffer_EINVAL) { int r; errno = 0; tc_iph_count = 0; r = _ultoa_s(v_ul, NULL, 10, 10); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ultoa_size_zero_EINVAL) { char b[8] = "q"; int r; errno = 0; tc_iph_count = 0; r = _ultoa_s(v_ul, b, 0, 10); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 'q'); }
TC_CASE(ultoa_radix_1_EINVAL) { char b[40]; int r; errno = 0; tc_iph_count = 0; r = _ultoa_s(v_ul, b, sizeof b, 1); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ltoa_radix_37_EINVAL) { char b[40]; int r; errno = 0; tc_iph_count = 0; r = _ltoa_s(v_l, b, sizeof b, 37); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ltoa_null_buffer_EINVAL) { int r; errno = 0; tc_iph_count = 0; r = _ltoa_s(v_l, NULL, 10, 10); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(handler_counts)
{
    char b[40]; tc_iph_count = 0;
    _ultoa_s(v_ul, NULL, 10, 10); _ultoa_s(v_ul, b, 0, 10); _ultoa_s(v_ul, b, sizeof b, 1);
    _ltoa_s(v_l, b, sizeof b, 37); _ltoa_s(v_l, NULL, 10, 10);
    CHECK_EQ_I(tc_iph_count, 5);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(ultoa_null_buffer_EINVAL); TC_RUN(ultoa_size_zero_EINVAL); TC_RUN(ultoa_radix_1_EINVAL);
    TC_RUN(ltoa_radix_37_EINVAL); TC_RUN(ltoa_null_buffer_EINVAL); TC_RUN(handler_counts);
    return tc_end();
}

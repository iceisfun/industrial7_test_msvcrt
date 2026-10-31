/* TC
 * id:    sec_neg_itow_s_args
 * area:  secure_crt
 * funcs: _itow_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _itow_s (Parameter validation table)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

static volatile int v_i = 255;

TC_CASE(null_buffer_EINVAL) { int r; errno = 0; r = _itow_s(v_i, NULL, 10, 10); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(size_zero_EINVAL) { wchar_t b[8] = L"q"; int r; errno = 0; r = _itow_s(v_i, b, 0, 10); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(b[0], L'q'); }
TC_CASE(radix_1_EINVAL) { wchar_t b[40]; int r; errno = 0; r = _itow_s(v_i, b, 40, 1); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(radix_37_EINVAL) { wchar_t b[40]; int r; errno = 0; r = _itow_s(v_i, b, 40, 37); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(handler_counts)
{
    wchar_t b[40]; tc_iph_count = 0;
    _itow_s(v_i, NULL, 10, 10); _itow_s(v_i, b, 0, 10); _itow_s(v_i, b, 40, 1); _itow_s(v_i, b, 40, 37);
    CHECK_EQ_I(tc_iph_count, 4);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(null_buffer_EINVAL); TC_RUN(size_zero_EINVAL); TC_RUN(radix_1_EINVAL); TC_RUN(radix_37_EINVAL); TC_RUN(handler_counts);
    return tc_end();
}

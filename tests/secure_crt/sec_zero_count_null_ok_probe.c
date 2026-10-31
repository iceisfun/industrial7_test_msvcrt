/* TC
 * id:    sec_zero_count_null_ok_probe
 * area:  secure_crt
 * funcs: memcpy_s memmove_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs memcpy_s / memmove_s error table
 * note:  PROBE: with count == 0, memcpy_s/memmove_s are believed to return 0 immediately (before pointer validation),
 *        even with NULL dest and no invalid-parameter handler call. The docs table lists NULL dest as EINVAL, so this checks which holds.
 */
#include "tc.h"
#include <string.h>

static volatile size_t v_zero;
TC_CASE(memcpy_s_null_dest_size0_null_src_count0_returns0_no_handler)
{ tc_iph_count = 0; CHECK_EQ_I(memcpy_s(NULL, v_zero, NULL, v_zero), 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(memcpy_s_null_dest_size8_valid_src_count0_returns0)
{ tc_iph_count = 0; CHECK_EQ_I(memcpy_s(NULL, 8 + v_zero, "a", v_zero), 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(memmove_s_same)
{ tc_iph_count = 0; CHECK_EQ_I(memmove_s(NULL, v_zero, NULL, v_zero), 0); CHECK_EQ_I(memmove_s(NULL, 8 + v_zero, "a", v_zero), 0); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(memcpy_s_null_dest_size0_null_src_count0_returns0_no_handler);
    TC_RUN(memcpy_s_null_dest_size8_valid_src_count0_returns0); TC_RUN(memmove_s_same);
    return tc_end();
}

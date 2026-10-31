/* TC
 * id:    sec_memcpy_s_basic
 * area:  secure_crt
 * funcs: memcpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs memcpy_s
 */
#include "tc.h"
#include <string.h>

TC_CASE(copy_exact_destsize) { static const char s[4] = {1,2,3,4}; char d[4] = {0}; volatile size_t n = 4; CHECK_EQ_I(memcpy_s(d, 4, s, n), 0); CHECK_MEM(d, s, 4); }
TC_CASE(copy_less_than_destsize_leaves_tail) { static const char s[2] = {7,8}; char d[5]; volatile size_t n = 2; memset(d, 0x55, sizeof d);
    CHECK_EQ_I(memcpy_s(d, sizeof d, s, n), 0); CHECK_EQ_I(d[0], 7); CHECK_EQ_I(d[1], 8); CHECK_EQ_I(d[2], 0x55); CHECK_EQ_I(d[4], 0x55); }
TC_CASE(embedded_nul_bytes_copied) { static const char s[5] = {'a',0,'b',0,'c'}; char d[5]; volatile size_t n = 5; memset(d, 'x', sizeof d); CHECK_EQ_I(memcpy_s(d, sizeof d, s, n), 0); CHECK_MEM(d, s, 5); }
TC_CASE(count_zero_returns_0_dest_untouched) { char d[3] = {9,9,9}, s[3] = {1,2,3}; volatile size_t n = 0; CHECK_EQ_I(memcpy_s(d, 3, s, n), 0); CHECK_EQ_I(d[0], 9); CHECK_EQ_I(d[2], 9); }

int main(void) { tc_begin(); TC_RUN(copy_exact_destsize); TC_RUN(copy_less_than_destsize_leaves_tail); TC_RUN(embedded_nul_bytes_copied); TC_RUN(count_zero_returns_0_dest_untouched); return tc_end(); }

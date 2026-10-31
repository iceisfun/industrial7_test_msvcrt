/* TC
 * id:    sec_neg_memmove_s_range
 * area:  secure_crt
 * funcs: memmove_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memmove_s: ERANGE when count > destSize, invalid parameter handler invoked
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

#define RESET() do { tc_iph_count = 0; errno = 0; } while (0)
static volatile size_t v_n;

TC_CASE(count_gt_destsize_ERANGE) { char d[4] = "abc", s[8] = "12345678"; v_n = 8; RESET();
    CHECK_EQ_I(memmove_s(d, sizeof d, s, v_n), ERANGE); }
TC_CASE(errno_ERANGE) { char d[4] = "abc", s[8] = "12345678"; v_n = 5; RESET();
    memmove_s(d, sizeof d, s, v_n); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(handler_count_one) { char d[4] = "abc", s[8] = "12345678"; v_n = 5; RESET();
    memmove_s(d, sizeof d, s, v_n); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(overlapping_but_too_large_still_ERANGE) { char b[8] = "abcdefg"; v_n = 6; RESET();
    CHECK_EQ_I(memmove_s(b, 4, b + 1, v_n), ERANGE); CHECK_EQ_I(tc_iph_count, 1); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(count_gt_destsize_ERANGE); TC_RUN(errno_ERANGE); TC_RUN(handler_count_one); TC_RUN(overlapping_but_too_large_still_ERANGE); return tc_end(); }

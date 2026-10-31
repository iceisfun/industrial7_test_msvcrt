/* TC
 * id:    sec_neg_wmemcpy_s_range
 * area:  secure_crt
 * funcs: wmemcpy_s
 * procs: wmemmove_s
 * crt:   >=100
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wmemcpy_s, wmemmove_s
 * note:  counts and sizes are in wchar_t elements
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

#define RESET() do { tc_iph_count = 0; errno = 0; } while (0)
typedef int (__cdecl *wmm_t)(wchar_t *, size_t, const wchar_t *, size_t);
static wmm_t p_wmemmove_s;
static volatile size_t v_n;

TC_CASE(count_gt_destsize_ERANGE) { wchar_t d[4] = {1, 2, 3, 4}, s[8] = {0}; v_n = 5; RESET();
    CHECK_EQ_I(wmemcpy_s(d, 4, s, v_n), ERANGE); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(null_dest_EINVAL) { wchar_t s[4] = {0}; v_n = 2; RESET();
    CHECK_EQ_I(wmemcpy_s(NULL, 4, s, v_n), EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(null_src_EINVAL) { wchar_t d[4] = {1, 2, 3, 4}; v_n = 2; RESET();
    CHECK_EQ_I(wmemcpy_s(d, 4, NULL, v_n), EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(wmemmove_s_range_ERANGE) { wchar_t d[4] = {1, 2, 3, 4}, s[8] = {0}; v_n = 5; RESET();
    if (!p_wmemmove_s) TC_SKIP("wmemmove_s not exported");
    CHECK_EQ_I(p_wmemmove_s(d, 4, s, v_n), ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    RESET(); CHECK_EQ_I(p_wmemmove_s(NULL, 4, s, 2), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(handler_counts) { wchar_t d[4] = {1, 2, 3, 4}, s[8] = {0}; v_n = 5; RESET();
    wmemcpy_s(d, 4, s, v_n); wmemcpy_s(NULL, 4, s, 2); wmemcpy_s(d, 4, NULL, 2);
    CHECK_EQ_I(tc_iph_count, 3); RESET(); CHECK_EQ_I(wmemcpy_s(d, 4, s, 4), 0); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3; p_wmemmove_s = (wmm_t)tc_crt_proc("wmemmove_s");
    TC_RUN(count_gt_destsize_ERANGE); TC_RUN(null_dest_EINVAL); TC_RUN(null_src_EINVAL); TC_RUN(wmemmove_s_range_ERANGE); TC_RUN(handler_counts); return tc_end(); }

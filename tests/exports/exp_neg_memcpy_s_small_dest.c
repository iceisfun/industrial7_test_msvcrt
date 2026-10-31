/* TC
 * id:    exp_neg_memcpy_s_small_dest
 * area:  exports
 * funcs:
 * procs: memcpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memcpy_s: destSize < count -> ERANGE, dest cleared
 */
#include "tc.h"

TC_DECL_PROC(int, memcpy_s, (void *, size_t, const void *, size_t));
TC_CASE(count_gt_destsize_erange) {
    tc_iph_count = 0;
    unsigned char d[4] = {9,9,9,9}; static const unsigned char s[8] = {1,2,3,4,5,6,7,8};
    CHECK_EQ_I(tc_p_memcpy_s(d, 4, s, 8), 34);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(dest_zeroed) {
    unsigned char d[4] = {9,9,9,9}; static const unsigned char s[8] = {1,2,3,4,5,6,7,8}; int i;
    tc_p_memcpy_s(d, 4, s, 8);
    for (i = 0; i < 4; i++) CHECK_EQ_I(d[i], 0);
}
TC_CASE(exact_fit_ok) {
    unsigned char d[4] = {9,9,9,9}; static const unsigned char s[4] = {1,2,3,4};
    CHECK_EQ_I(tc_p_memcpy_s(d, 4, s, 4), 0);
    CHECK_MEM(d, s, 4);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(memcpy_s);
    CHECK(!tc_p_memcpy_s ? 0 : 1);
    if (!tc_p_memcpy_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    CHECK(tc_iph_install());
    TC_RUN(count_gt_destsize_erange);
    TC_RUN(dest_zeroed);
    TC_RUN(exact_fit_ok);
    return tc_end();
}

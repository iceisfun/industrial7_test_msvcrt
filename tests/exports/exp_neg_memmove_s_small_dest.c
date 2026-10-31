/* TC
 * id:    exp_neg_memmove_s_small_dest
 * area:  exports
 * funcs:
 * procs: memmove_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memmove_s, wmemmove_s
 * note:  Dest state after failure NOT asserted.
 */
#include "tc.h"

TC_DECL_PROC(int, memmove_s, (void *, size_t, const void *, size_t));
TC_CASE(small_dest_erange) {
    tc_iph_count = 0;
    unsigned char d[4]; static const unsigned char s[8] = {1,2,3,4,5,6,7,8};
    CHECK_EQ_I(tc_p_memmove_s(d, 4, s, 8), 34);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_dest_einval) {
    tc_iph_count = 0;
    static const unsigned char s[4] = {1,2,3,4};
    CHECK_EQ_I(tc_p_memmove_s(NULL, 4, s, 4), 22);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_src_einval) {
    tc_iph_count = 0;
    unsigned char d[4];
    CHECK_EQ_I(tc_p_memmove_s(d, 4, NULL, 4), 22);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(memmove_s);
    CHECK(!tc_p_memmove_s ? 0 : 1);
    if (!tc_p_memmove_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    CHECK(tc_iph_install());
    TC_RUN(small_dest_erange);
    TC_RUN(null_dest_einval);
    TC_RUN(null_src_einval);
    return tc_end();
}

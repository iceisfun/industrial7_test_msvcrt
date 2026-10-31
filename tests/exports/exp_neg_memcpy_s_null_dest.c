/* TC
 * id:    exp_neg_memcpy_s_null_dest
 * area:  exports
 * funcs:
 * procs: memcpy_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memcpy_s
 */
#include "tc.h"

TC_DECL_PROC(int, memcpy_s, (void *, size_t, const void *, size_t));
TC_CASE(null_dest_einval) {
    tc_iph_count = 0;
    static const unsigned char s[4] = {1,2,3,4};
    CHECK_EQ_I(tc_p_memcpy_s(NULL, 4, s, 4), 22);
}
TC_CASE(iph_called) {
    tc_iph_count = 0;
    static const unsigned char s[4] = {1,2,3,4};
    tc_p_memcpy_s(NULL, 4, s, 4);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(memcpy_s);
    CHECK(!tc_p_memcpy_s ? 0 : 1);
    if (!tc_p_memcpy_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    CHECK(tc_iph_install());
    TC_RUN(null_dest_einval);
    TC_RUN(iph_called);
    return tc_end();
}

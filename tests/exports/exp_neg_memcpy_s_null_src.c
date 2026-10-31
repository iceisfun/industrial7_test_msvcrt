/* TC
 * id:    exp_neg_memcpy_s_null_src
 * area:  exports
 * funcs:
 * procs: memcpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memcpy_s, wmemcpy_s: error table (src NULL -> EINVAL, dest zeroed)
 */
#include "tc.h"

TC_DECL_PROC(int, memcpy_s, (void *, size_t, const void *, size_t));
TC_CASE(null_src_einval_dest_zeroed) {
    tc_iph_count = 0;
    unsigned char d[10]; int i;
    for (i = 0; i < 10; i++) d[i] = 0x55;
    CHECK_EQ_I(tc_p_memcpy_s(d, 10, NULL, 5), 22);
    for (i = 0; i < 5; i++) CHECK_EQ_I(d[i], 0);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(count0_null_src_ok) {
    tc_iph_count = 0;
    unsigned char d[4] = {1,2,3,4};
    CHECK_EQ_I(tc_p_memcpy_s(d, 4, NULL, 0), 0);
    CHECK_EQ_I(d[0], 1);
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(memcpy_s);
    CHECK(!tc_p_memcpy_s ? 0 : 1);
    if (!tc_p_memcpy_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    CHECK(tc_iph_install());
    TC_RUN(null_src_einval_dest_zeroed);
    TC_RUN(count0_null_src_ok);
    return tc_end();
}

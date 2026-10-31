/* TC
 * id:    exp_neg_strcpy_s_null_dest
 * area:  exports
 * funcs:
 * procs: strcpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcpy_s, strcpy_s_l, wcscpy_s (error conditions table)
 */
#include "tc.h"

TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));
TC_CASE(null_dest_einval) {
    tc_iph_count = 0;
    volatile const char *s = "a";
    CHECK_EQ_I(tc_p_strcpy_s(NULL, 10, (const char *)s), 22);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_src_einval_and_dest_cleared) {
    tc_iph_count = 0;
    char d[8] = "xyz";
    CHECK_EQ_I(tc_p_strcpy_s(d, sizeof d, NULL), 22);
    CHECK_EQ_I(d[0], 0);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(zero_size_einval) {
    tc_iph_count = 0;
    char d[8] = "xyz";
    volatile const char *s = "a";
    CHECK_EQ_I(tc_p_strcpy_s(d, 0, (const char *)s), 22);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(strcpy_s);
    CHECK(!tc_p_strcpy_s ? 0 : 1);
    if (!tc_p_strcpy_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    CHECK(tc_iph_install());
    TC_RUN(null_dest_einval);
    TC_RUN(null_src_einval_and_dest_cleared);
    TC_RUN(zero_size_einval);
    return tc_end();
}

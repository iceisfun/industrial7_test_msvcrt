/* TC
 * id:    exp_neg_strncpy_s_count_ge_size
 * area:  exports
 * funcs:
 * procs: strncpy_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncpy_s
 */
#include "tc.h"

TC_DECL_PROC(int, strncpy_s, (char *, size_t, const char *, size_t));
TC_CASE(count10_size4_erange) {
    tc_iph_count = 0;
    char d[4] = "abc"; volatile const char *s = "hello";
    CHECK_EQ_I(tc_p_strncpy_s(d, 4, (const char *)s, 10), 34);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(dest0_nul) {
    char d[4] = "abc"; volatile const char *s = "hello";
    tc_p_strncpy_s(d, 4, (const char *)s, 10);
    CHECK_EQ_I(d[0], 0);
}
TC_CASE(null_dest_einval) {
    tc_iph_count = 0;
    volatile const char *s = "hello";
    CHECK_EQ_I(tc_p_strncpy_s(NULL, 4, (const char *)s, 2), 22);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(strncpy_s);
    CHECK(!tc_p_strncpy_s ? 0 : 1);
    if (!tc_p_strncpy_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    CHECK(tc_iph_install());
    TC_RUN(count10_size4_erange);
    TC_RUN(dest0_nul);
    TC_RUN(null_dest_einval);
    return tc_end();
}

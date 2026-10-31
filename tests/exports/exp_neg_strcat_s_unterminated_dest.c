/* TC
 * id:    exp_neg_strcat_s_unterminated_dest
 * area:  exports
 * funcs:
 * procs: strcat_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcat_s, wcscat_s
 */
#include "tc.h"

TC_DECL_PROC(int, strcat_s, (char *, size_t, const char *));
TC_CASE(dest_not_terminated_einval) {
    tc_iph_count = 0;
    char d[4] = {'a','b','c','d'};
    volatile const char *s = "x";
    CHECK_EQ_I(tc_p_strcat_s(d, 4, (const char *)s), 22);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(too_small_erange) {
    tc_iph_count = 0;
    char d[4] = "ab";
    volatile const char *s = "cde";
    CHECK_EQ_I(tc_p_strcat_s(d, 4, (const char *)s), 34);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_args_einval) {
    tc_iph_count = 0;
    char d[4] = "ab";
    volatile const char *s = "c";
    CHECK_EQ_I(tc_p_strcat_s(NULL, 4, (const char *)s), 22);
    CHECK_EQ_I(tc_p_strcat_s(d, 4, NULL), 22);
    CHECK_EQ_I(tc_iph_count, 2);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(strcat_s);
    CHECK(!tc_p_strcat_s ? 0 : 1);
    if (!tc_p_strcat_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    CHECK(tc_iph_install());
    TC_RUN(dest_not_terminated_einval);
    TC_RUN(too_small_erange);
    TC_RUN(null_args_einval);
    return tc_end();
}

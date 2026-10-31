/* TC
 * id:    exp_neg_strcpy_s_small_buffer
 * area:  exports
 * funcs:
 * procs: strcpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcpy_s: ERANGE when destination too small; 'hello' needs 6 bytes
 */
#include "tc.h"

TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));
TC_CASE(size3_hello_erange) {
    tc_iph_count = 0;
    char d[8] = "xxxxxxx";
    volatile const char *s = "hello";
    CHECK_EQ_I(tc_p_strcpy_s(d, 3, (const char *)s), 34);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(dest0_set_to_nul) {
    char d[8] = "xxxxxxx";
    volatile const char *s = "hello";
    tc_p_strcpy_s(d, 3, (const char *)s);
    CHECK_EQ_I(d[0], 0);
}
TC_CASE(exact_fit_ok_size6) {
    char d[8] = "xxxxxxx";
    volatile const char *s = "hello";
    CHECK_EQ_I(tc_p_strcpy_s(d, 6, (const char *)s), 0);
    CHECK_STR(d, "hello");
}
TC_CASE(off_by_one_size5_erange) {
    tc_iph_count = 0;
    char d[8] = "xxxxxxx";
    volatile const char *s = "hello";
    CHECK_EQ_I(tc_p_strcpy_s(d, 5, (const char *)s), 34);
    CHECK_EQ_I(d[0], 0);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(strcpy_s);
    CHECK(!tc_p_strcpy_s ? 0 : 1);
    if (!tc_p_strcpy_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    CHECK(tc_iph_install());
    TC_RUN(size3_hello_erange);
    TC_RUN(dest0_set_to_nul);
    TC_RUN(exact_fit_ok_size6);
    TC_RUN(off_by_one_size5_erange);
    return tc_end();
}

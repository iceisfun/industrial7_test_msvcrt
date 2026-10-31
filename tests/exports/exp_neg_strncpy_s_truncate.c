/* TC
 * id:    exp_neg_strncpy_s_truncate
 * area:  exports
 * funcs:
 * procs: strncpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncpy_s, _strncpy_s_l: _TRUNCATE copies as much as fits, returns STRUNCATE
 */
#include "tc.h"

TC_DECL_PROC(int, strncpy_s, (char *, size_t, const char *, size_t));
TC_CASE(truncate_returns_strunc_80) {
    char d[4]; volatile const char *s = "hello";
    CHECK_EQ_I(tc_p_strncpy_s(d, 4, (const char *)s, (size_t)-1), 80);
}
TC_CASE(dest_terminated_hel) {
    char d[4]; volatile const char *s = "hello";
    tc_p_strncpy_s(d, 4, (const char *)s, (size_t)-1);
    CHECK_STR(d, "hel");
}
TC_CASE(no_truncation_returns_0) {
    char d[8]; volatile const char *s = "hi";
    CHECK_EQ_I(tc_p_strncpy_s(d, 8, (const char *)s, (size_t)-1), 0);
    CHECK_STR(d, "hi");
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(strncpy_s);
    CHECK(!tc_p_strncpy_s ? 0 : 1);
    if (!tc_p_strncpy_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    CHECK(tc_iph_install());
    TC_RUN(truncate_returns_strunc_80);
    TC_RUN(dest_terminated_hel);
    TC_RUN(no_truncation_returns_0);
    return tc_end();
}

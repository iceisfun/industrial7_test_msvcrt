/* TC
 * id:    str_neg_strcpy_s_family_invalid_msvcr80
 * area:  string
 * funcs:
 * procs: strcpy_s strcat_s _set_invalid_parameter_handler
 * crt:   msvcr80
 * tier:  p2
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcpy_s, strcat_s (NULL dest / zero size / too small => EINVAL or ERANGE and invalid parameter handler)
 */
#include "tc.h"

TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));
TC_DECL_PROC(int, strcat_s, (char *, size_t, const char *));
#define EINVAL_ 22
#define ERANGE_ 34

TC_CASE(null_dest)
{
    int r;
    if (!tc_p_strcpy_s) TC_SKIP("no strcpy_s");
    tc_iph_count = 0;
    r = tc_p_strcpy_s(NULL, 10, "a");
    CHECK_EQ_I(r, EINVAL_); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_src_clears_dest)
{
    char b[8] = "zz";
    int r;
    if (!tc_p_strcpy_s) TC_SKIP("no strcpy_s");
    tc_iph_count = 0;
    r = tc_p_strcpy_s(b, sizeof b, NULL);
    CHECK_EQ_I(r, EINVAL_); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(too_small)
{
    char b[4] = "zzz";
    int r;
    if (!tc_p_strcpy_s) TC_SKIP("no strcpy_s");
    tc_iph_count = 0;
    r = tc_p_strcpy_s(b, 3, "abc");
    CHECK_EQ_I(r, ERANGE_); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(cat_overflow)
{
    char b[4] = "ab";
    int r;
    if (!tc_p_strcat_s) TC_SKIP("no strcat_s");
    tc_iph_count = 0;
    r = tc_p_strcat_s(b, 4, "cde");
    CHECK_EQ_I(r, ERANGE_); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(strcpy_s); TC_LOAD_PROC(strcat_s);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph api\n"); return 1; }
    TC_RUN(null_dest); TC_RUN(null_src_clears_dest); TC_RUN(too_small); TC_RUN(cat_overflow);
    return tc_end();
}

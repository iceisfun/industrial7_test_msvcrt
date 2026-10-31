/* TC
 * id:    sec_neg_core_via_procs
 * area:  secure_crt
 * kind:  negative
 * funcs:
 * procs: strcpy_s memcpy_s strcat_s _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs strcpy_s, strcat_s, memcpy_s error tables; Parameter Validation
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));
TC_DECL_PROC(int, memcpy_s, (void *, size_t, const void *, size_t));
TC_DECL_PROC(int, strcat_s, (char *, size_t, const char *));

TC_CASE(strcpy_s_null_dest_EINVAL) { tc_iph_count = 0; CHECK_EQ_I(tc_p_strcpy_s(NULL, 4, "a"), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(strcpy_s_small_ERANGE_empty)
{ char d[3] = "xx"; tc_iph_count = 0; CHECK_EQ_I(tc_p_strcpy_s(d, 3, "abcdef"), ERANGE); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(strcat_s_unterminated_dest_EINVAL)
{ char d[4] = {'a','b','c','d'}; tc_iph_count = 0; CHECK_EQ_I(tc_p_strcat_s(d, 4, "x"), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(memcpy_s_count_gt_dest_ERANGE)
{ char d[4] = {1,1,1,1}; tc_iph_count = 0; CHECK_EQ_I(tc_p_memcpy_s(d, 2, "abcd", 4), ERANGE); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(handler_count_matches)
{ char d[2]; tc_iph_count = 0; tc_p_strcpy_s(NULL, 4, "a"); tc_p_strcpy_s(d, 2, "abc"); tc_p_memcpy_s(d, 1, "ab", 2); CHECK_EQ_I(tc_iph_count, 3); }
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(strcpy_s); TC_LOAD_PROC(memcpy_s); TC_LOAD_PROC(strcat_s);
    if (!tc_iph_install() || !tc_p_strcpy_s || !tc_p_memcpy_s || !tc_p_strcat_s) { tc_write("TC FAIL " TC_TEST_ID "::load missing export\n"); return 1; }
    TC_RUN(strcpy_s_null_dest_EINVAL); TC_RUN(strcpy_s_small_ERANGE_empty); TC_RUN(strcat_s_unterminated_dest_EINVAL);
    TC_RUN(memcpy_s_count_gt_dest_ERANGE); TC_RUN(handler_count_matches); return tc_end();
}

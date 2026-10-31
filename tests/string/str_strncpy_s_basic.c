/* TC
 * id:    str_strncpy_s_basic
 * area:  string
 * funcs: 
 * procs: strncpy_s
 * crt:   >=100
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strncpy_s, _strncpy_s_l
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
typedef int (__cdecl *pncpy_t)(char *, size_t, const char *, size_t);
static pncpy_t p_ncpy;

TC_CASE(count_gt_len) { char d[16]; volatile const char *s = "abc"; memset(d, 'X', sizeof d); CHECK_EQ_I(p_ncpy(d, sizeof d, (const char *)s, 10), 0); CHECK_STR(d, "abc"); }
TC_CASE(count_lt_len) { char d[16]; volatile const char *s = "abcdef"; memset(d, 'X', sizeof d); CHECK_EQ_I(p_ncpy(d, sizeof d, (const char *)s, 3), 0); CHECK_STR(d, "abc"); CHECK_EQ_I(d[3], 0); }
TC_CASE(count_eq_len) { char d[8]; volatile const char *s = "abcd"; CHECK_EQ_I(p_ncpy(d, sizeof d, (const char *)s, 4), 0); CHECK_STR(d, "abcd"); }
TC_CASE(count_zero) { char d[8]; volatile const char *s = "abcd"; memset(d, 'X', sizeof d); CHECK_EQ_I(p_ncpy(d, sizeof d, (const char *)s, 0), 0); CHECK_EQ_I(d[0], 0); }
TC_CASE(empty_src) { char d[8]; volatile const char *s = ""; memset(d, 'X', sizeof d); CHECK_EQ_I(p_ncpy(d, sizeof d, (const char *)s, 5), 0); CHECK_EQ_I(d[0], 0); }
int main(void)
{
    tc_begin(); tc_iph_install();
    p_ncpy = (pncpy_t)tc_crt_proc("strncpy_s");
    if (!p_ncpy) { tc_write("TC FAIL " TC_TEST_ID "::__import strncpy_s missing\n"); return 1; }
    TC_RUN(count_gt_len); TC_RUN(count_lt_len); TC_RUN(count_eq_len); TC_RUN(count_zero); TC_RUN(empty_src);
    return tc_end();
}

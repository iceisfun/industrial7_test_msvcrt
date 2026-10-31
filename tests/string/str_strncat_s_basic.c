/* TC
 * id:    str_strncat_s_basic
 * area:  string
 * funcs: 
 * procs: strncat_s
 * crt:   >=100
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strncat_s
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
typedef int (__cdecl *pnc_t)(char *, size_t, const char *, size_t);
static pnc_t p_nc;

TC_CASE(append_n) { char d[16]; volatile const char *s = "defgh"; strcpy(d, "abc"); CHECK_EQ_I(p_nc(d, sizeof d, (const char *)s, 2), 0); CHECK_STR(d, "abcde"); }
TC_CASE(count_gt_len) { char d[16]; volatile const char *s = "def"; strcpy(d, "abc"); CHECK_EQ_I(p_nc(d, sizeof d, (const char *)s, 100), 0); CHECK_STR(d, "abcdef"); }
TC_CASE(count_zero) { char d[16]; volatile const char *s = "def"; strcpy(d, "abc"); CHECK_EQ_I(p_nc(d, sizeof d, (const char *)s, 0), 0); CHECK_STR(d, "abc"); }
TC_CASE(truncate_fits) { char d[16]; volatile const char *s = "def"; strcpy(d, "abc"); CHECK_EQ_I(p_nc(d, sizeof d, (const char *)s, (size_t)-1), 0); CHECK_STR(d, "abcdef"); }
int main(void)
{
    tc_begin(); tc_iph_install();
    p_nc = (pnc_t)tc_crt_proc("strncat_s");
    if (!p_nc) { tc_write("TC FAIL " TC_TEST_ID "::__import strncat_s missing\n"); return 1; }
    TC_RUN(append_n); TC_RUN(count_gt_len); TC_RUN(count_zero); TC_RUN(truncate_fits);
    return tc_end();
}

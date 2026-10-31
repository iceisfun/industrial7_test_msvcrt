/* TC
 * id:    str_neg_strncat_s_invalid
 * area:  string
 * funcs: 
 * procs: strncat_s
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncat_s error conditions (dest NULL, size 0, src NULL with count>0)
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
typedef int (__cdecl *pnc_t)(char *, size_t, const char *, size_t);
static pnc_t p_nc;

TC_CASE(null_dest) { volatile const char *s = "abc"; errno = 0; tc_iph_count = 0; CHECK_EQ_I(p_nc(NULL, 8, (const char *)s, 3), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_src) { char d[8]; volatile const char *s = NULL; strcpy(d, "ab"); errno = 0; tc_iph_count = 0; CHECK_EQ_I(p_nc(d, sizeof d, (const char *)s, 3), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(zero_size) { char d[8]; volatile const char *s = "abc"; strcpy(d, "ab"); errno = 0; tc_iph_count = 0; CHECK_EQ_I(p_nc(d, 0, (const char *)s, 3), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    p_nc = (pnc_t)tc_crt_proc("strncat_s");
    if (!p_nc) { tc_write("TC FAIL " TC_TEST_ID "::__import strncat_s missing\n"); return 1; }
    TC_RUN(null_dest); TC_RUN(null_src); TC_RUN(zero_size);
    return tc_end();
}

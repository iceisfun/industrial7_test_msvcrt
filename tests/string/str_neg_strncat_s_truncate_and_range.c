/* TC
 * id:    str_neg_strncat_s_truncate_and_range
 * area:  string
 * funcs: 
 * procs: strncat_s
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncat_s (STRUNCATE, ERANGE, EINVAL)
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
typedef int (__cdecl *pnc_t)(char *, size_t, const char *, size_t);
static pnc_t p_nc;

TC_CASE(truncate_not_fitting) { char d[8]; volatile const char *s = "defghijk"; strcpy(d, "abc"); errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(p_nc(d, sizeof d, (const char *)s, (size_t)-1), 80); CHECK_STR(d, "abcdefg"); }
TC_CASE(count_too_large) { char d[8]; volatile const char *s = "defghijk"; strcpy(d, "abc"); errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(p_nc(d, sizeof d, (const char *)s, 8), ERANGE); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(unterminated_dest) { char d[4]; volatile const char *s = "x"; memset(d, 'A', sizeof d); errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(p_nc(d, sizeof d, (const char *)s, 1), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    p_nc = (pnc_t)tc_crt_proc("strncat_s");
    if (!p_nc) { tc_write("TC FAIL " TC_TEST_ID "::__import strncat_s missing\n"); return 1; }
    TC_RUN(truncate_not_fitting); TC_RUN(count_too_large); TC_RUN(unterminated_dest);
    return tc_end();
}

/* TC
 * id:    str_neg_strcat_s_too_small
 * area:  string
 * funcs: 
 * procs: strcat_s
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcat_s, wcscat_s, _mbscat_s (ERANGE: destination buffer too small)
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
typedef int (__cdecl *pcat_t)(char *, size_t, const char *);
static pcat_t p_cat;

TC_CASE(erange_and_empty)
{
    char d[8]; volatile const char *s = "defgh"; int r;
    strcpy(d, "abc"); tc_iph_count = 0;
    r = p_cat(d, sizeof d, (const char *)s);   /* needs 3+5+1 = 9 > 8 */
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(d[0], 0);
}
TC_CASE(errno_and_iph)
{
    char d[8]; volatile const char *s = "defgh"; int r;
    strcpy(d, "abc"); tc_iph_count = 0; errno = 0;
    r = p_cat(d, sizeof d, (const char *)s);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(off_by_one)
{
    char d[8]; volatile const char *s = "defg"; volatile const char *t = "defgh"; int r;
    strcpy(d, "abc"); tc_iph_count = 0;
    r = p_cat(d, sizeof d, (const char *)s);   /* 3+4+1 = 8 fits exactly */
    CHECK_EQ_I(r, 0); CHECK_STR(d, "abcdefg"); CHECK_EQ_I(tc_iph_count, 0);
    strcpy(d, "abc");
    r = p_cat(d, sizeof d, (const char *)t);   /* one more does not */
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(control_success)
{
    char d[16]; volatile const char *s = "def"; int r;
    strcpy(d, "abc"); tc_iph_count = 0; errno = 0;
    r = p_cat(d, sizeof d, (const char *)s);
    CHECK_EQ_I(r, 0); CHECK_STR(d, "abcdef"); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    p_cat = (pcat_t)tc_crt_proc("strcat_s");
    if (!p_cat) { tc_write("TC FAIL " TC_TEST_ID "::__import strcat_s missing\n"); return 1; }
    TC_RUN(erange_and_empty); TC_RUN(errno_and_iph); TC_RUN(off_by_one); TC_RUN(control_success);
    return tc_end();
}

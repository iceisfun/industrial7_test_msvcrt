/* TC
 * id:    str_neg_strncpy_s_invalid
 * area:  string
 * funcs: 
 * procs: strncpy_s
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncpy_s error conditions table (dest NULL / size 0 -> EINVAL; src NULL -> EINVAL and dest[0]=0)
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
typedef int (__cdecl *pncpy_t)(char *, size_t, const char *, size_t);
static pncpy_t p_ncpy;

TC_CASE(null_dest) { volatile const char *s = "abc"; errno = 0; tc_iph_count = 0; CHECK_EQ_I(p_ncpy(NULL, 8, (const char *)s, 3), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_src) { char d[8]; volatile const char *s = NULL; memset(d, 'X', sizeof d); errno = 0; tc_iph_count = 0; CHECK_EQ_I(p_ncpy(d, sizeof d, (const char *)s, 3), EINVAL); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(zero_size) { char d[4]; volatile const char *s = "abc"; memset(d, 'X', sizeof d); tc_iph_count = 0; CHECK_EQ_I(p_ncpy(d, 0, (const char *)s, 3), EINVAL); CHECK_EQ_I(d[0], 'X'); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    p_ncpy = (pncpy_t)tc_crt_proc("strncpy_s");
    if (!p_ncpy) { tc_write("TC FAIL " TC_TEST_ID "::__import strncpy_s missing\n"); return 1; }
    TC_RUN(null_dest); TC_RUN(null_src); TC_RUN(zero_size);
    return tc_end();
}

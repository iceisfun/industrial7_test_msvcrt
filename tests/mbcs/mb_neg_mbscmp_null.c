/* TC
 * id:    mb_neg_mbscmp_null
 * area:  mbcs
 * funcs: _mbscmp _mbsicmp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mbscmp: returns _NLSCMPERROR (0x7fffffff) on error, errno EINVAL
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>

#define NLSERR 0x7fffffff

TC_CASE(cmp_null_first)  { static unsigned char b[] = "a"; tc_iph_count = 0; CHECK_EQ_I(_mbscmp(NULL, b), NLSERR); CHECK(tc_iph_count >= 1); }
TC_CASE(cmp_null_second) { static unsigned char a[] = "a"; tc_iph_count = 0; CHECK_EQ_I(_mbscmp(a, NULL), NLSERR); CHECK(tc_iph_count >= 1); }
TC_CASE(icmp_null_both)  { tc_iph_count = 0; CHECK_EQ_I(_mbsicmp(NULL, NULL), NLSERR); CHECK(tc_iph_count >= 1); }
TC_CASE(errno_set)       { static unsigned char b[] = "a"; errno = 0; _mbscmp(NULL, b); CHECK_EQ_I(errno, EINVAL); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no iph\n"); return 1; }
    TC_RUN(cmp_null_first); TC_RUN(cmp_null_second); TC_RUN(icmp_null_both); TC_RUN(errno_set);
    return tc_end();
}

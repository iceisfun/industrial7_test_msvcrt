/* TC
 * id:    str_neg_strnicmp_null_retval_errno
 * area:  string
 * funcs: _strnicmp
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strnicmp: "returns _NLSCMPERROR on an error" (INT_MAX, string.h); parameter validation sets errno to EINVAL and calls the invalid parameter handler
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <limits.h>
static const char *volatile pn = NULL;
static const char *volatile pabc = "abc";

TC_CASE(first_null) { errno = 0; tc_iph_count = 0; CHECK_EQ_I(_strnicmp(pn, pabc, 3), INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(second_null) { errno = 0; tc_iph_count = 0; CHECK_EQ_I(_strnicmp(pabc, pn, 3), INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(both_null) { errno = 0; tc_iph_count = 0; CHECK_EQ_I(_strnicmp(pn, pn, 3), INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(valid_after_error) { errno = 0; tc_iph_count = 0; CHECK_EQ_I(_strnicmp(pabc, "ABC", 3), 0); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(first_null); TC_RUN(second_null); TC_RUN(both_null); TC_RUN(valid_after_error);
    return tc_end();
}

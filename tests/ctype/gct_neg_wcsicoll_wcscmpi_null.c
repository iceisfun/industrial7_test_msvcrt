/* TC
 * id:    gct_neg_wcsicoll_wcscmpi_null
 * area:  ctype
 * funcs: _wcsicoll _wcsicmp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _wcsicmp, strcoll family (_NLSCMPERROR with EINVAL on NULL)
 * note:  wcscmpi (old alias of _wcsicmp) is an inline in UCRT headers, not an export; _wcsicmp is tested.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static const wchar_t * volatile nul = NULL;
static const wchar_t * volatile a = L"a";

#define EXPECT_ERR(call) do { int r; int b = tc_iph_count; errno = 0; r = (call); \
    CHECK_EQ_I(r, INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, b + 1); } while (0)

TC_CASE(wcsicoll_first_null)  { EXPECT_ERR(_wcsicoll(nul, a)); }
TC_CASE(wcsicoll_second_null) { EXPECT_ERR(_wcsicoll(a, nul)); }
TC_CASE(wcsicoll_both_null)   { EXPECT_ERR(_wcsicoll(nul, nul)); }
TC_CASE(_wcsicmp_first_null)   { EXPECT_ERR(_wcsicmp(nul, a)); }
TC_CASE(_wcsicmp_second_null)  { EXPECT_ERR(_wcsicmp(a, nul)); }
TC_CASE(_wcsicmp_both_null)    { EXPECT_ERR(_wcsicmp(nul, nul)); }
TC_CASE(valid_after_failure)
{
    CHECK_EQ_I(_wcsicoll(a, L"A"), 0);
    CHECK_EQ_I(_wcsicmp(a, L"A"), 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(wcsicoll_first_null); TC_RUN(wcsicoll_second_null); TC_RUN(wcsicoll_both_null);
    TC_RUN(_wcsicmp_first_null); TC_RUN(_wcsicmp_second_null); TC_RUN(_wcsicmp_both_null); TC_RUN(valid_after_failure);
    return tc_end();
}

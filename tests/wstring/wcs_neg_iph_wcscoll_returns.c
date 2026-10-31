/* TC
 * id:    wcs_neg_iph_wcscoll_returns
 * area:  wstring
 * funcs: wcscoll
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcscoll (return _NLSCMPERROR = INT_MAX on error, errno EINVAL); Parameter validation
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static const wchar_t *volatile vs = L"abc", *volatile vn = NULL;

TC_CASE(wcscoll_returns_intmax)
{
    int r; tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = wcscoll(vn, vs);
    CHECK_EQ_I(r, INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wcscoll_second_null)
{
    int r; tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = wcscoll(vs, vn);
    CHECK_EQ_I(r, INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(valid_call_no_handler)
{
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(wcscoll(vs, vs), 0);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
int main(void) { tc_begin(); TC_RUN(wcscoll_returns_intmax); TC_RUN(wcscoll_second_null); TC_RUN(valid_call_no_handler); return tc_end(); }

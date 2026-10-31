/* TC
 * id:    wcs_neg_iph_returns_after_custom
 * area:  wstring
 * funcs: _wcsicmp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _wcsicmp (return _NLSCMPERROR = INT_MAX on error, errno EINVAL); Parameter validation
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static const wchar_t *volatile vs = L"abc", *volatile vn = NULL;

TC_CASE(wcsicmp_returns_intmax)
{
    int r; tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = _wcsicmp(vn, vs);
    CHECK_EQ_I(r, INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wcsicmp_count_per_call)
{
    tc_iph_install(); tc_iph_count = 0;
    _wcsicmp(vs, vn); CHECK_EQ_I(tc_iph_count, 1);
    _wcsicmp(vn, vn); CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(valid_call_no_handler)
{
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_wcsicmp(vs, vs), 0);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
int main(void) { tc_begin(); TC_RUN(wcsicmp_returns_intmax); TC_RUN(wcsicmp_count_per_call); TC_RUN(valid_call_no_handler); return tc_end(); }

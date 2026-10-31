/* TC
 * id:    wcs_neg_wcsnicmp_null
 * area:  wstring
 * funcs: _wcsnicmp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strnicmp, _wcsnicmp: returns _NLSCMPERROR (INT_MAX) on error; Parameter Validation
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static const wchar_t *volatile vs = L"abc", *volatile vn = NULL;
TC_CASE(null_first) { int r; tc_iph_install(); tc_iph_count = 0; errno = 0; r = _wcsnicmp(vn, vs, 3);
    CHECK_EQ_I(r, 0x7fffffff); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_second) { int r; tc_iph_install(); tc_iph_count = 0; errno = 0; r = _wcsnicmp(vs, vn, 3);
    CHECK_EQ_I(r, 0x7fffffff); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_both) { int r; tc_iph_install(); tc_iph_count = 0; errno = 0; r = _wcsnicmp(vn, vn, 3);
    CHECK_EQ_I(r, 0x7fffffff); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); TC_RUN(null_first); TC_RUN(null_second); TC_RUN(null_both); return tc_end(); }

/* TC
 * id:    wcs_neg_wcsicmp_null
 * area:  wstring
 * funcs: _wcsicmp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _stricmp, _wcsicmp: returns _NLSCMPERROR (INT_MAX) on error; Parameter Validation
 * note:  validation was added in the 8.0 CRT
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static const wchar_t *volatile vs = L"abc", *volatile vn = NULL;
TC_CASE(null_first) { int r; tc_iph_install(); tc_iph_count = 0; errno = 0; r = _wcsicmp(vn, vs);
    CHECK_EQ_I(r, 0x7fffffff); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_second) { int r; tc_iph_install(); tc_iph_count = 0; errno = 0; r = _wcsicmp(vs, vn);
    CHECK_EQ_I(r, 0x7fffffff); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_both) { int r; tc_iph_install(); tc_iph_count = 0; errno = 0; r = _wcsicmp(vn, vn);
    CHECK_EQ_I(r, 0x7fffffff); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(count_accumulates) { tc_iph_install(); tc_iph_count = 0; _wcsicmp(vn, vs); _wcsicmp(vs, vn); _wcsicmp(vn, vn); CHECK_EQ_I(tc_iph_count, 3); }
int main(void) { tc_begin(); TC_RUN(null_first); TC_RUN(null_second); TC_RUN(null_both); TC_RUN(count_accumulates); return tc_end(); }

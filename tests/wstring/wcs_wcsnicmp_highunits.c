/* TC
 * id:    wcs_wcsnicmp_highunits
 * area:  wstring
 * funcs: _wcsnicmp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _wcsnicmp
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static int nicmp(const wchar_t *a, const wchar_t *b, size_t n)
{ const wchar_t *volatile pa = a, *volatile pb = b; volatile size_t vn = n; return _wcsnicmp(pa, pb, vn); }
TC_CASE(pair_equal) { static const wchar_t a[] = { 0xD83D, 0xDE00, 0 }, b[] = { 0xD83D, 0xDE00, 0 }; CHECK_EQ_I(nicmp(a, b, 2), 0); }
TC_CASE(ffff_equal) { static const wchar_t a[] = { 0xFFFF, 0 }, b[] = { 0xFFFF, 0 }; CHECK_EQ_I(nicmp(a, b, 1), 0); }
TC_CASE(ffff_vs_a) { static const wchar_t a[] = { 0xFFFF, 0 }, b[] = { 'a', 0 }; CHECK(nicmp(a, b, 1) > 0); CHECK(nicmp(b, a, 1) < 0); }
TC_CASE(lone_surrogate) { static const wchar_t a[] = { 0xD800, 0 }, b[] = { 0xD800, 0 }; CHECK_EQ_I(nicmp(a, b, 1), 0); }
int main(void) { tc_begin(); TC_RUN(pair_equal); TC_RUN(ffff_equal); TC_RUN(ffff_vs_a); TC_RUN(lone_surrogate); return tc_end(); }

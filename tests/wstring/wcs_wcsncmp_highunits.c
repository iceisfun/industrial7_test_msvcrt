/* TC
 * id:    wcs_wcsncmp_highunits
 * area:  wstring
 * funcs: wcsncmp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.4.3; MS docs wcsncmp
 * note:  wchar_t is unsigned 16-bit; compares code units, not code points
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static int ncmp(const wchar_t *a, const wchar_t *b, size_t n)
{ const wchar_t *volatile pa = a, *volatile pb = b; volatile size_t vn = n; return wcsncmp(pa, pb, vn); }
TC_CASE(ffff_vs_7fff) { static const wchar_t a[] = { 0xFFFF, 0 }, b[] = { 0x7FFF, 0 };
    CHECK(ncmp(a, b, 1) > 0); CHECK(ncmp(b, a, 1) < 0); CHECK(ncmp(a, b, 5) > 0); }
TC_CASE(surrogate_trail_diff) { static const wchar_t a[] = { 0xD83D, 0xDE00, 0 }, b[] = { 0xD83D, 0xDE01, 0 };
    CHECK(ncmp(a, b, 2) < 0); CHECK(ncmp(b, a, 2) > 0); CHECK(ncmp(a, a, 2) == 0); }
TC_CASE(n1_equal_on_lead) { static const wchar_t a[] = { 0xD83D, 0xDE00, 0 }, b[] = { 0xD83D, 0xDE01, 0 };
    CHECK_EQ_I(ncmp(a, b, 1), 0); }
int main(void) { tc_begin(); TC_RUN(ffff_vs_7fff); TC_RUN(surrogate_trail_diff); TC_RUN(n1_equal_on_lead); return tc_end(); }

/* TC
 * id:    wcs_wcsicmp_c_locale_nonascii
 * area:  wstring
 * funcs: _wcsicmp
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _wcsicmp (locale dependent)
 * note:  PROBE: in the C locale _wcsicmp folds only ASCII A-Z, so U+00C0 != U+00E0 and U+0410 != U+0430. Older CRTs may use towlower with CT_CTYPE tables and fold more. Real-Windows probe.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static int icmp1(wchar_t a, wchar_t b)
{ wchar_t x[2], y[2]; const wchar_t *volatile px = x, *volatile py = y; x[0] = a; x[1] = 0; y[0] = b; y[1] = 0; return _wcsicmp(px, py); }
TC_CASE(latin1) { CHECK(icmp1(0x00C0, 0x00E0) != 0); CHECK(icmp1(0x00C0, 0x00E0) < 0); }
TC_CASE(cyrillic) { CHECK(icmp1(0x0410, 0x0430) != 0); CHECK(icmp1(0x0410, 0x0430) < 0); }
TC_CASE(ffff_equal) { CHECK_EQ_I(icmp1(0xFFFF, 0xFFFF), 0); }
int main(void) { tc_begin(); TC_RUN(latin1); TC_RUN(cyrillic); TC_RUN(ffff_equal); return tc_end(); }

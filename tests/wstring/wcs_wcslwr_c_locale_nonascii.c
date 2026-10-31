/* TC
 * id:    wcs_wcslwr_c_locale_nonascii
 * area:  wstring
 * funcs: _wcslwr _wcsupr
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _wcslwr, _wcsupr
 * note:  PROBE: in the "C" locale, non-ASCII letters (U+00C0, U+00E0, U+0410, U+0430) are believed to be left unchanged
 *        by _wcslwr/_wcsupr. Older CRTs might map via LCMapString regardless of locale.
 */
#include "tc.h"
#include <wchar.h>

TC_CASE(latin_upper_lwr_stays) { wchar_t b[] = { 0x00C0, 0 }; wchar_t *volatile p = b; _wcslwr(p); CHECK_EQ_U(b[0], 0x00C0); }
TC_CASE(latin_lower_upr_stays) { wchar_t b[] = { 0x00E0, 0 }; wchar_t *volatile p = b; _wcsupr(p); CHECK_EQ_U(b[0], 0x00E0); }
TC_CASE(cyrillic_upper_lwr_stays) { wchar_t b[] = { 0x0410, 0 }; wchar_t *volatile p = b; _wcslwr(p); CHECK_EQ_U(b[0], 0x0410); }
TC_CASE(cyrillic_lower_upr_stays) { wchar_t b[] = { 0x0430, 0 }; wchar_t *volatile p = b; _wcsupr(p); CHECK_EQ_U(b[0], 0x0430); }

int main(void) { tc_begin(); TC_RUN(latin_upper_lwr_stays); TC_RUN(latin_lower_upr_stays); TC_RUN(cyrillic_upper_lwr_stays); TC_RUN(cyrillic_lower_upr_stays); return tc_end(); }

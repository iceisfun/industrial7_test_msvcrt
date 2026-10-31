/* TC
 * id:    wcs_neg_wcslwr_surrogates_unchanged
 * area:  wstring
 * funcs: _wcslwr _wcsupr _wcsicmp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _wcslwr, _wcsupr, _wcsicmp (C locale: only A-Z / a-z are case-mapped); UTF-16 code units are mapped individually
 * note:  Probes that, in the C locale, surrogate code units and U+FFFE/U+FFFF are never case-mapped and are not folded
 *        together by _wcsicmp, and that Deseret U+10400 (D801 DC00) is not folded with U+10428 (D801 DC28).
 */
#include "tc.h"
#include <wchar.h>

TC_CASE(lwr_pair_unchanged)  { wchar_t s[] = { 0xD801, 0xDC00, L'A', 0 }; _wcslwr(s); CHECK_EQ_I(s[0], 0xD801); CHECK_EQ_I(s[1], 0xDC00); CHECK_EQ_I(s[2], L'a'); }
TC_CASE(upr_pair_unchanged)  { wchar_t s[] = { 0xD801, 0xDC28, L'a', 0 }; _wcsupr(s); CHECK_EQ_I(s[0], 0xD801); CHECK_EQ_I(s[1], 0xDC28); CHECK_EQ_I(s[2], L'A'); }
TC_CASE(lone_hi_lo_unchanged)
{
    wchar_t s[] = { 0xD800, L'X', 0xDC00, 0xFFFE, 0xFFFF, 0 };
    _wcslwr(s); CHECK_EQ_I(s[0], 0xD800); CHECK_EQ_I(s[1], L'x'); CHECK_EQ_I(s[2], 0xDC00); CHECK_EQ_I(s[3], 0xFFFE); CHECK_EQ_I(s[4], 0xFFFF);
    _wcsupr(s); CHECK_EQ_I(s[0], 0xD800); CHECK_EQ_I(s[2], 0xDC00); CHECK_EQ_I(s[3], 0xFFFE); CHECK_EQ_I(s[4], 0xFFFF);
}
TC_CASE(icmp_lone_hi_vs_lo_nonzero)
{
    wchar_t a[] = { 0xD800, 0 }, b[] = { 0xDC00, 0 };
    CHECK(_wcsicmp(a, b) != 0); CHECK(_wcsicmp(b, a) != 0);
    CHECK_EQ_I(_wcsicmp(a, a), 0);
}
TC_CASE(icmp_ffff_vs_fffe_nonzero)
{
    wchar_t a[] = { 0xFFFF, 0 }, b[] = { 0xFFFE, 0 };
    CHECK(_wcsicmp(a, b) != 0); CHECK(_wcsicmp(b, a) != 0);
}
TC_CASE(deseret_pair_not_folded)
{
    wchar_t up[] = { 0xD801, 0xDC00, 0 }, lo[] = { 0xD801, 0xDC28, 0 };
    CHECK(_wcsicmp(up, lo) != 0);
}
int main(void)
{
    tc_begin(); TC_RUN(lwr_pair_unchanged); TC_RUN(upr_pair_unchanged); TC_RUN(lone_hi_lo_unchanged);
    TC_RUN(icmp_lone_hi_vs_lo_nonzero); TC_RUN(icmp_ffff_vs_fffe_nonzero); TC_RUN(deseret_pair_not_folded);
    return tc_end();
}

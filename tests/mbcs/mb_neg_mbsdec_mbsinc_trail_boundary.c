/* TC
 * id:    mb_neg_mbsdec_mbsinc_trail_boundary
 * area:  mbcs
 * funcs: _mbsdec _mbsnextc _mbclen _setmbcp _getmbcp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mbsdec, _mbsnextc, _mbclen
 * note:  cp932. A lone lead byte before NUL is deliberately NOT asserted (see mb_probe_mbsinc_dangling_lead).
 *        _mbsinc stepping over a NUL is undocumented: see mb_probe_mbsinc_at_nul.
 *        mbsdec at a trail byte: only "never before start" is asserted.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static int old_cp;
static const unsigned char *volatile v_s, *volatile v_cur;

TC_CASE(dec_never_before_start)
{
    static const unsigned char s[] = {0x82, 0xA0, 0};
    const unsigned char *r;
    v_s = s; v_cur = s + 1;           /* positioned on the trail byte */
    r = _mbsdec(v_s, v_cur);
    CHECK(r == NULL || r >= s);
    v_cur = s;
    CHECK(_mbsdec(v_s, v_cur) == NULL);
}
TC_CASE(dec_over_ascii_after_dbcs)
{
    static const unsigned char s[] = {0x82, 0xA0, 'a', 0};
    v_s = s; v_cur = s + 3; CHECK(_mbsdec(v_s, v_cur) == s + 2);
    v_cur = s + 2;          CHECK(_mbsdec(v_s, v_cur) == s);
}
TC_CASE(nextc_at_nul_zero)
{
    static const unsigned char s[] = {0};
    v_s = s;
    CHECK_EQ_U(_mbsnextc(v_s), 0);
}
TC_CASE(clen_on_ascii_one)
{
    static const unsigned char a[] = "a", d[] = {0x82, 0xA0, 0};
    v_s = a; CHECK_EQ_U(_mbclen(v_s), 1);
    v_s = d; CHECK_EQ_U(_mbclen(v_s), 2);
    CHECK_EQ_U(_mbsnextc(v_s), 0x82A0);
}
int main(void)
{
    tc_begin(); old_cp = _getmbcp(); _setmbcp(932);
    TC_RUN(dec_never_before_start); TC_RUN(dec_over_ascii_after_dbcs); TC_RUN(nextc_at_nul_zero); TC_RUN(clen_on_ascii_one);
    _setmbcp(old_cp);
    return tc_end();
}

/* TC
 * id:    mb_neg_mbsstr_empty_needle_edge
 * area:  mbcs
 * funcs: _mbsstr _mbschr _mbsnbcnt _setmbcp _getmbcp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mbsstr (empty needle returns str1; NULL if not found), _mbschr (NUL matches terminator), _mbsnbcnt
 * note:  cp932. Misaligned matches (trail byte of one char + lead byte of the next) must not be reported.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static int old_cp;
static const unsigned char *volatile v_hay;
static volatile unsigned int v_c;

TC_CASE(empty_needle_returns_haystack)
{
    static const unsigned char h[] = {0x82, 0xA0, 'a', 0}, n[] = {0};
    v_hay = h;
    CHECK(_mbsstr(v_hay, n) == h);
}
TC_CASE(needle_longer_returns_null)
{
    static const unsigned char h[] = "ab", n[] = "abc";
    v_hay = h;
    CHECK(_mbsstr(v_hay, n) == NULL);
}
TC_CASE(misaligned_dbcs_not_matched_932)
{
    /* chars: 82A0 82A2 ; the byte pair A0 82 straddles them */
    static const unsigned char h[] = {0x82, 0xA0, 0x82, 0xA2, 0}, n[] = {0xA0, 0x82, 0};
    v_hay = h;
    CHECK(_mbsstr(v_hay, n) == NULL);
}
TC_CASE(chr_dbcs_misaligned)
{
    /* chars: 8182 then single-byte kana A2 ; the pair 82A2 straddles them */
    static const unsigned char h[] = {0x81, 0x82, 0xA2, 0};
    v_hay = h; v_c = 0x82A2;
    CHECK(_mbschr(v_hay, v_c) == NULL);
}
TC_CASE(chr_nul_returns_terminator)
{
    static const unsigned char h[] = {0x82, 0xA0, 'a', 0};
    v_hay = h; v_c = 0;
    CHECK(_mbschr(v_hay, v_c) == h + 3);
}
TC_CASE(nbcnt_stops_at_nul)
{
    static const unsigned char h[] = {0x82, 0xA0, 'a', 0};
    v_hay = h;
    CHECK_EQ_U(_mbsnbcnt(v_hay, 2), 3);
    CHECK_EQ_U(_mbsnbcnt(v_hay, 50), 3);
    CHECK_EQ_U(_mbsnbcnt(v_hay, 0), 0);
}
int main(void)
{
    tc_begin(); old_cp = _getmbcp(); _setmbcp(932);
    TC_RUN(empty_needle_returns_haystack); TC_RUN(needle_longer_returns_null); TC_RUN(misaligned_dbcs_not_matched_932);
    TC_RUN(chr_dbcs_misaligned); TC_RUN(chr_nul_returns_terminator); TC_RUN(nbcnt_stops_at_nul);
    _setmbcp(old_cp);
    return tc_end();
}

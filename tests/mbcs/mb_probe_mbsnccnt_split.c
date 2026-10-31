/* TC
 * id:    mb_probe_mbsnccnt_split
 * area:  mbcs
 * funcs: _mbsnccnt _mbsnbcnt _setmbcp
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _mbsnccnt
 * note:  PROBE: (1) nbytes ends between lead and trail byte: the partial character is expected not to be counted; (2) _mbsnbcnt on a string ending in a lead byte before NUL: byte count is in 1..3 (2 likely; 3 if stepping past NUL; 1 if the dangling lead is not counted).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

TC_CASE(split_char)
{
    static const unsigned char s[] = { 0x82, 0xA0, 0x82, 0xA2, 0 };
    static const unsigned char t[] = { 0x82, 0xA0, 'a', 0x82, 0xA2, 0 };
    if (_setmbcp(932) != 0) TC_SKIP("cp 932 unavailable");
    CHECK_EQ_U(_mbsnccnt((const unsigned char *volatile)s, 3), 1);
    CHECK_EQ_U(_mbsnccnt((const unsigned char *volatile)t, 4), 2);
    _setmbcp(_MB_CP_ANSI);
}
TC_CASE(dangling_lead)
{
    static const unsigned char s[] = { 'a', 0x82, 0 };
    size_t r;
    if (_setmbcp(932) != 0) TC_SKIP("cp 932 unavailable");
    r = _mbsnbcnt((const unsigned char *volatile)s, 2);
    CHECK(r >= 1 && r <= 3);
    _setmbcp(_MB_CP_ANSI);
}
int main(void) { tc_begin(); TC_RUN(split_char); TC_RUN(dangling_lead); return tc_end(); }

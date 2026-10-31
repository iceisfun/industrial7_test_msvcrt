/* TC
 * id:    mb_mbsnccnt_basic
 * area:  mbcs
 * funcs: _mbsnccnt _setmbcp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _mbsnccnt, _mbsnccnt_l
 * note:  cp 932; byte counts that end mid-character are in mb_probe_mbsnccnt_split
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

TC_CASE(counts)
{
    static const unsigned char s[] = { 0x82, 0xA0, 'a', 0x82, 0xA2, 0 };
    const unsigned char *volatile p = s;
    if (_setmbcp(932) != 0) TC_SKIP("cp 932 unavailable");
    CHECK_EQ_U(_mbsnccnt(p, 0), 0);
    CHECK_EQ_U(_mbsnccnt(p, 2), 1);
    CHECK_EQ_U(_mbsnccnt(p, 3), 2);
    CHECK_EQ_U(_mbsnccnt(p, 5), 3);
    CHECK_EQ_U(_mbsnccnt(p, 100), 3);
    _setmbcp(_MB_CP_ANSI);
}
int main(void) { tc_begin(); TC_RUN(counts); return tc_end(); }

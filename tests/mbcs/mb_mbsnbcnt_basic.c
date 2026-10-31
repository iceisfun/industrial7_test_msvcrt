/* TC
 * id:    mb_mbsnbcnt_basic
 * area:  mbcs
 * funcs: _mbsnbcnt _setmbcp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _mbsnbcnt, _mbsnbcnt_l
 * note:  cp 932
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

TC_CASE(counts)
{
    static const unsigned char s[] = { 0x82, 0xA0, 'a', 0x82, 0xA2, 0 };
    const unsigned char *volatile p = s;
    if (_setmbcp(932) != 0) TC_SKIP("cp 932 unavailable");
    CHECK_EQ_U(_mbsnbcnt(p, 0), 0);
    CHECK_EQ_U(_mbsnbcnt(p, 1), 2);
    CHECK_EQ_U(_mbsnbcnt(p, 2), 3);
    CHECK_EQ_U(_mbsnbcnt(p, 3), 5);
    CHECK_EQ_U(_mbsnbcnt(p, 10), 5);
    _setmbcp(_MB_CP_ANSI);
}
int main(void) { tc_begin(); TC_RUN(counts); return tc_end(); }

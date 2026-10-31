/* TC
 * id:    mb_mbsinc_dec_basic
 * area:  mbcs
 * funcs: _mbsinc _mbsdec _setmbcp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _mbsinc, _mbsdec
 * note:  cp 932
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static unsigned char s[] = { 0x82, 0xA0, 'a', 0x82, 0xA2, 0 };

TC_CASE(inc)
{
    unsigned char *volatile p = s;
    if (_setmbcp(932) != 0) TC_SKIP("cp 932 unavailable");
    CHECK_EQ_P(_mbsinc(p), s + 2);
    CHECK_EQ_P(_mbsinc(p + 2), s + 3);
    CHECK_EQ_P(_mbsinc(p + 3), s + 5);
    _setmbcp(_MB_CP_ANSI);
}
TC_CASE(dec)
{
    unsigned char *volatile p = s;
    if (_setmbcp(932) != 0) TC_SKIP("cp 932 unavailable");
    CHECK_EQ_P(_mbsdec(p, p + 2), s);
    CHECK_EQ_P(_mbsdec(p, p + 3), s + 2);
    CHECK_EQ_P(_mbsdec(p, p + 5), s + 3);
    _setmbcp(_MB_CP_ANSI);
}
int main(void) { tc_begin(); TC_RUN(inc); TC_RUN(dec); return tc_end(); }

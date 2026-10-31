/* TC
 * id:    mb_mbsninc_nextc
 * area:  mbcs
 * funcs: _mbsninc _mbsnextc _mbclen _setmbcp
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _mbsninc, _mbsnextc, _mbclen
 * note:  cp 932
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static unsigned char s[] = { 0x82, 0xA0, 'a', 0x82, 0xA2, 0 };

TC_CASE(ninc)
{
    unsigned char *volatile p = s;
    if (_setmbcp(932) != 0) TC_SKIP("cp 932 unavailable");
    CHECK_EQ_P(_mbsninc(p, 0), s);
    CHECK_EQ_P(_mbsninc(p, 1), s + 2);
    CHECK_EQ_P(_mbsninc(p, 2), s + 3);
    CHECK_EQ_P(_mbsninc(p, 3), s + 5);
    _setmbcp(_MB_CP_ANSI);
}
TC_CASE(nextc)
{
    unsigned char *volatile p = s;
    if (_setmbcp(932) != 0) TC_SKIP("cp 932 unavailable");
    CHECK_EQ_U(_mbsnextc(p), 0x82A0);
    CHECK_EQ_U(_mbsnextc(p + 2), 0x61);
    CHECK_EQ_U(_mbsnextc(p + 3), 0x82A2);
    _setmbcp(_MB_CP_ANSI);
}
TC_CASE(clen)
{
    unsigned char *volatile p = s;
    if (_setmbcp(932) != 0) TC_SKIP("cp 932 unavailable");
    CHECK_EQ_U(_mbclen(p), 2);
    CHECK_EQ_U(_mbclen(p + 2), 1);
    _setmbcp(_MB_CP_ANSI);
}
int main(void) { tc_begin(); TC_RUN(ninc); TC_RUN(nextc); TC_RUN(clen); return tc_end(); }

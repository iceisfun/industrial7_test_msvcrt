/* TC
 * id:    mb_mbsncmp_family
 * area:  mbcs
 * funcs: _mbsncmp _mbsnbcmp _mbsnicmp _mbsnbicmp _setmbcp
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _mbsnbcmp, _mbsncmp, _mbsnicmp, _mbsnbicmp
 * note:  Count 0 returns 0.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>

TC_CASE(chars) { static unsigned char a[] = "abcd", b[] = "abxx"; CHECK_EQ_I(_mbsncmp(a, b, 2), 0); CHECK(_mbsncmp(a, b, 3) < 0); }
TC_CASE(bytes3)
{
    static unsigned char a[] = { 0x82, 0xA0, 0x82, 0xA2, 0 }, b[] = { 0x82, 0xA0, 0x82, 0xA3, 0 };
    CHECK_EQ_I(_mbsnbcmp(a, b, 3), 0);
}
TC_CASE(bytes4)
{
    static unsigned char a[] = { 0x82, 0xA0, 0x82, 0xA2, 0 }, b[] = { 0x82, 0xA0, 0x82, 0xA3, 0 };
    CHECK(_mbsnbcmp(a, b, 4) < 0);
}
TC_CASE(zero) { static unsigned char a[] = "a", b[] = "b"; CHECK_EQ_I(_mbsncmp(a, b, 0), 0); CHECK_EQ_I(_mbsnbcmp(a, b, 0), 0); }
TC_CASE(icase)
{
    static unsigned char a[] = "ABcd", b[] = "abCX";
    CHECK_EQ_I(_mbsnicmp(a, b, 3), 0); CHECK_EQ_I(_mbsnbicmp(a, b, 3), 0);
    CHECK(_mbsnicmp(a, b, 4) != 0);
}

int main(void)
{
    int old = _getmbcp();
    tc_begin(); _setmbcp(932);
    TC_RUN(chars); TC_RUN(bytes3); TC_RUN(bytes4); TC_RUN(zero); TC_RUN(icase);
    _setmbcp(old); return tc_end();
}

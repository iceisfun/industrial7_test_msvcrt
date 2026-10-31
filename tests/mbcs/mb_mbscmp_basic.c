/* TC
 * id:    mb_mbscmp_basic
 * area:  mbcs
 * funcs: _mbscmp _mbsicmp _setmbcp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mbscmp, _mbscmp_l, _mbsicmp
 * note:  Only the sign of the result is asserted.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>

TC_CASE(equal) { static unsigned char a[] = "abc", b[] = "abc"; CHECK_EQ_I(_mbscmp(a, b), 0); }
TC_CASE(less) { static unsigned char a[] = "a", b[] = "b"; CHECK(_mbscmp(a, b) < 0); CHECK(_mbscmp(b, a) > 0); }
TC_CASE(unsigned_high)
{
    static unsigned char a[] = { 0x81, 0x40, 0 }, b[] = "A";
    CHECK(_mbscmp(a, b) > 0);
}
TC_CASE(prefix) { static unsigned char a[] = "abc", b[] = "abcd"; CHECK(_mbscmp(a, b) < 0); CHECK(_mbscmp(b, a) > 0); }
TC_CASE(icase) { static unsigned char a[] = "ABC", b[] = "abc"; CHECK_EQ_I(_mbsicmp(a, b), 0); CHECK(_mbscmp(a, b) < 0); }

int main(void)
{
    int old = _getmbcp();
    tc_begin(); _setmbcp(932);
    TC_RUN(equal); TC_RUN(less); TC_RUN(unsigned_high); TC_RUN(prefix); TC_RUN(icase);
    _setmbcp(old); return tc_end();
}

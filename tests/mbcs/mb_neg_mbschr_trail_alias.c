/* TC
 * id:    mb_neg_mbschr_trail_alias
 * area:  mbcs
 * funcs: _mbschr _mbsrchr _setmbcp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _mbschr (searches by multibyte character)
 * note:  A single-byte search value equal to a DBCS trail byte must not match inside the double-byte char.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>

TC_CASE(at_sign)
{
    static unsigned char s[] = { 0x81, 0x40, 0 }; volatile unsigned int c = 0x40;
    CHECK(_mbschr(s, c) == NULL); CHECK(_mbsrchr(s, c) == NULL);
}
TC_CASE(backslash)
{
    static unsigned char s[] = { 0x82, 0x5C, 0 }; volatile unsigned int c = '\\';
    CHECK(_mbschr(s, c) == NULL); CHECK(_mbsrchr(s, c) == NULL);
}
TC_CASE(lead_alias_not_trail)
{
    static unsigned char s[] = { 0x82, 0xA0, 0 }; volatile unsigned int c = 0xA0;
    CHECK(_mbschr(s, c) == NULL);
}

int main(void)
{
    int old = _getmbcp();
    tc_begin(); _setmbcp(932);
    TC_RUN(at_sign); TC_RUN(backslash); TC_RUN(lead_alias_not_trail);
    _setmbcp(old); return tc_end();
}

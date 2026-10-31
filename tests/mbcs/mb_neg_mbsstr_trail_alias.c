/* TC
 * id:    mb_neg_mbsstr_trail_alias
 * area:  mbcs
 * funcs: _mbsstr _setmbcp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _mbsstr (multibyte-aware)
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>

TC_CASE(trail_then_ascii)
{
    static unsigned char h[] = { 0x82, 0x40, 0x41, 0 }, n[] = "@A";
    CHECK(_mbsstr(h, n) == NULL);
}
TC_CASE(backslash_trail)
{
    static unsigned char h[] = { 0x81, 0x5C, 0 }, n[] = "\\";
    CHECK(_mbsstr(h, n) == NULL);
}
TC_CASE(trail_only_needle)
{
    static unsigned char h[] = { 0x82, 0xA0, 0 }, n[] = { 0xA0, 0 };
    CHECK(_mbsstr(h, n) == NULL);
}

int main(void)
{
    int old = _getmbcp();
    tc_begin(); _setmbcp(932);
    TC_RUN(trail_then_ascii); TC_RUN(backslash_trail); TC_RUN(trail_only_needle);
    _setmbcp(old); return tc_end();
}

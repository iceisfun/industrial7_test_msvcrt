/* TC
 * id:    mb_mbschr_basic
 * area:  mbcs
 * funcs: _mbschr _mbsrchr _setmbcp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mbschr, _mbsrchr; _setmbcp
 * note:  Code page 932 (Shift-JIS) is selected explicitly.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>

static int old_cp;
static void setup(void) { old_cp = _getmbcp(); _setmbcp(932); }
static void restore(void) { _setmbcp(old_cp); }

TC_CASE(ascii)
{
    static unsigned char s[] = "abcb"; unsigned char *p;
    p = _mbschr(s, 'b'); CHECK(p == s + 1);
    p = _mbsrchr(s, 'b'); CHECK(p == s + 3);
}
TC_CASE(terminator)
{
    static unsigned char s[] = "abc"; volatile unsigned int z = 0;
    CHECK(_mbschr(s, z) == s + 3);
}
TC_CASE(missing)
{
    static unsigned char s[] = "abc"; volatile unsigned int c = 'z';
    CHECK(_mbschr(s, c) == NULL); CHECK(_mbsrchr(s, c) == NULL);
}
TC_CASE(dbcs)
{
    static unsigned char s[] = { 'a', 0x82, 0xA0, 'b', 0x82, 0xA0, 0 }; volatile unsigned int c = 0x82A0;
    CHECK(_mbschr(s, c) == s + 1);
    CHECK(_mbsrchr(s, c) == s + 4);
}
TC_CASE(trail_not_matched)
{
    static unsigned char s[] = { 0x81, 0x40, 0x40, 0 }; volatile unsigned int c = 0x40;
    CHECK(_mbschr(s, c) == s + 2);  /* first 0x40 is a trail byte; the second is a real char */
}

int main(void)
{
    tc_begin(); setup();
    TC_RUN(ascii); TC_RUN(terminator); TC_RUN(missing); TC_RUN(dbcs); TC_RUN(trail_not_matched);
    restore(); return tc_end();
}

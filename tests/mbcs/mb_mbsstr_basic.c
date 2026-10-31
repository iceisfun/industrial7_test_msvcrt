/* TC
 * id:    mb_mbsstr_basic
 * area:  mbcs
 * funcs: _mbsstr _setmbcp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mbsstr, wcsstr, strstr
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>

static const unsigned char *vp(const unsigned char *p) { return p; }

TC_CASE(ascii)
{
    static unsigned char h[] = "abcde", n[] = "cd";
    CHECK(_mbsstr(h, n) == h + 2);
}
TC_CASE(empty_needle)
{
    static unsigned char h[] = "abcde", n[] = "";
    CHECK(_mbsstr(h, n) == h);
}
TC_CASE(not_found)
{
    static unsigned char h[] = "abcde", n[] = "xy", n2[] = "ef";
    CHECK(_mbsstr(h, n) == NULL); CHECK(_mbsstr(h, n2) == NULL);
}
TC_CASE(dbcs_needle)
{
    static unsigned char h[] = { 0x82, 0xA1, 0x82, 0xA0, 0 }, n[] = { 0x82, 0xA0, 0 };
    CHECK(_mbsstr(h, n) == h + 2);
}

int main(void)
{
    int old = _getmbcp();
    tc_begin(); _setmbcp(932); (void)vp;
    TC_RUN(ascii); TC_RUN(empty_needle); TC_RUN(not_found); TC_RUN(dbcs_needle);
    _setmbcp(old); return tc_end();
}

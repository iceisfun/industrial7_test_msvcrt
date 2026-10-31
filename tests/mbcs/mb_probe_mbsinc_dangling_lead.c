/* TC
 * id:    mb_probe_mbsinc_dangling_lead
 * area:  mbcs
 * funcs: _mbsinc _setmbcp
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _mbsinc (valid text only)
 * note:  PROBE: _mbsinc on a lead byte followed by NUL; documented only for valid text. Asserts the result is s+1 or s+2 (likely s+1).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static unsigned char s[] = { 0x82, 0, 0 };

TC_CASE(lead_nul)
{
    unsigned char *volatile p = s;
    unsigned char *r;
    if (_setmbcp(932) != 0) TC_SKIP("cp 932 unavailable");
    r = _mbsinc(p);
    CHECK(r == s + 1 || r == s + 2);
    _setmbcp(_MB_CP_ANSI);
}
int main(void) { tc_begin(); TC_RUN(lead_nul); return tc_end(); }

/* TC
 * id:    mb_neg_mbsrchr_trail_alias
 * area:  mbcs
 * funcs: _mbsrchr _setmbcp _getmbcp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mbsrchr (multibyte-aware search; terminating NUL is considered part of the string)
 * note:  searching for 0 returns the terminator; a byte equal to the search char that is a DBCS trail byte is not a match.
 */
#include "tc.h"
#include <mbstring.h>
static int old_cp;
static unsigned char s[] = {'x',0x82,0x41,'y',0};   /* CP932: 0x82 0x41 is one character */
static unsigned char * volatile sp = s;
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(rchr_find_nul) { CHECK(_mbsrchr(sp, 0) == s + 4); }
TC_CASE(rchr_trail_alias_932)
{
    CHECK(_mbsrchr(sp, 'A') == NULL);
    CHECK(_mbsrchr(sp, 'y') == s + 3);
    CHECK(_mbsrchr(sp, 0x8241) == s + 1);
}
int main(void)
{
    tc_begin(); tc_iph_install(); old_cp = _getmbcp();
    TC_RUN(cp); TC_RUN(rchr_find_nul); TC_RUN(rchr_trail_alias_932);
    _setmbcp(old_cp); return tc_end();
}

/* TC
 * id:    mb_mbsnset_rev_misc
 * area:  mbcs
 * funcs: _mbsrev _mbsset _setmbcp _getmbcp
 * crt:   all
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _mbsrev (reverses characters, DBCS pairs keep byte order), _mbsset
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static int old_cp;
static void fill(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i] = 0xFF; }
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(rev)
{
    unsigned char s[] = {0x82,0xA0,'a',0x82,0xA2,0}, e[] = {0x82,0xA2,'a',0x82,0xA0,0};
    unsigned char *r = _mbsrev(s);
    CHECK_EQ_P(r, s);
    CHECK_MEM(s, e, sizeof e);
}
TC_CASE(rev_empty)
{
    unsigned char s[4] = {0, 'x'};
    _mbsrev(s);
    CHECK_EQ_U(s[0], 0); CHECK_EQ_U(s[1], 'x');
}
TC_CASE(set_sbcs)
{
    unsigned char s[] = "abc";
    _mbsset(s, 'x');
    CHECK_STR((char*)s, "xxx");
}
TC_CASE(set_dbcs)
{
    unsigned char s[] = "abcd", e[] = {0x82,0xA0,0x82,0xA0,0};
    _mbsset(s, 0x82A0);
    CHECK_MEM(s, e, sizeof e);
}
int main(void) { tc_begin(); old_cp = _getmbcp(); TC_RUN(cp); TC_RUN(rev); TC_RUN(rev_empty); TC_RUN(set_sbcs); TC_RUN(set_dbcs); _setmbcp(old_cp); return tc_end(); }

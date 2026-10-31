/* TC
 * id:    mb_mbsset_odd_probe
 * area:  mbcs
 * funcs: _mbsset _setmbcp _getmbcp
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _mbsset
 * note:  PROBE: filling an odd-length (3 byte) string with a DBCS char leaves the final byte as a space (0x20),
 *        not a lone lead byte, and the string stays 3 bytes long.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static int old_cp;
static void fill(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i] = 0xFF; }
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(odd)
{
    unsigned char s[] = "abc";
    _mbsset(s, 0x82A0);
    CHECK_EQ_U(s[0], 0x82); CHECK_EQ_U(s[1], 0xA0); CHECK_EQ_U(s[2], ' '); CHECK_EQ_U(s[3], 0);
}
int main(void) { tc_begin(); old_cp = _getmbcp(); TC_RUN(cp); TC_RUN(odd); _setmbcp(old_cp); return tc_end(); }

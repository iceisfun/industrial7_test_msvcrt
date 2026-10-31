/* TC
 * id:    mb_mbbtombc_932
 * area:  mbcs
 * funcs: _mbbtombc _mbctombb _setmbcp _getmbcp
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _mbbtombc, _mbctombb; JIS X 0208 / Shift-JIS table (cp932)
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static int old_cp;
static void fill(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i] = 0xFF; }
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(to_full)
{
    CHECK_EQ_U(_mbbtombc(0x41), 0x8260);
    CHECK_EQ_U(_mbbtombc(0x20), 0x8140);
    CHECK_EQ_U(_mbbtombc(0xB1), 0x8341);
}
TC_CASE(to_half)
{
    CHECK_EQ_U(_mbctombb(0x8260), 0x41);
    CHECK_EQ_U(_mbctombb(0x8341), 0xB1);
    CHECK_EQ_U(_mbctombb(0x8140), 0x20);
}
int main(void) { tc_begin(); old_cp = _getmbcp(); TC_RUN(cp); TC_RUN(to_full); TC_RUN(to_half); _setmbcp(old_cp); return tc_end(); }

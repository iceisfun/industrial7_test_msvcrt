/* TC
 * id:    mb_neg_mbbtombc_1252_probe
 * area:  mbcs
 * funcs: _mbbtombc _setmbcp _getmbcp
 * kind:  negative
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _mbbtombc
 * note:  PROBE: under multibyte code page 1252 (non-Japanese) _mbbtombc('A') returns 'A' unchanged,
 *        because the half/full-width mapping only applies to code page 932.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static int old_cp;
static void fill(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i] = 0xFF; }
TC_CASE(cp1252)
{
    CHECK_EQ_I(_setmbcp(1252), 0);
    CHECK_EQ_U(_mbbtombc('A'), 'A');
}
int main(void) { tc_begin(); old_cp = _getmbcp(); TC_RUN(cp1252); _setmbcp(old_cp); return tc_end(); }

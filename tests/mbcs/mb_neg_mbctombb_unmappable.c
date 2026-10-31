/* TC
 * id:    mb_neg_mbctombb_unmappable
 * area:  mbcs
 * funcs: _mbbtombc _mbctombb _setmbcp _getmbcp
 * kind:  negative
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _mbbtombc/_mbctombb: characters with no counterpart are returned unchanged
 * note:  cp932 only; the cp1252 behaviour is in mb_neg_mbbtombc_1252_probe
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static int old_cp;
static void fill(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i] = 0xFF; }
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(kanji)  { CHECK_EQ_U(_mbctombb(0x889F), 0x889F); }
TC_CASE(c80)    { CHECK_EQ_U(_mbbtombc(0x80), 0x80); }
int main(void) { tc_begin(); old_cp = _getmbcp(); TC_RUN(cp); TC_RUN(kanji); TC_RUN(c80); _setmbcp(old_cp); return tc_end(); }

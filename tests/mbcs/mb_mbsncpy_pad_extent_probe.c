/* TC
 * id:    mb_mbsncpy_pad_extent_probe
 * area:  mbcs
 * funcs: _mbsncpy _mbsnbcpy _setmbcp _getmbcp
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _mbsncpy
 * note:  PROBE (2): _mbsncpy/_mbsnbcpy with count==0 write nothing to dst (not documented explicitly).
 *        PROBE (1): with count (5 chars) > source length (2 chars = 3 bytes), the padding is count-srclen = 3 NUL BYTES
 *        (one NUL per missing character), so bytes 3..5 are 0 and byte 6 is untouched. Alternative: padded to count bytes.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static int old_cp;
static void fill(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i] = 0xFF; }
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(zero_count)
{
    unsigned char src[] = {0x82,0xA0,0}, dst[8], d2[8];
    fill(dst, 8); fill(d2, 8);
    _mbsncpy(dst, src, 0);
    CHECK_EQ_U(dst[0], 0xFF); CHECK_EQ_U(dst[1], 0xFF);
    _mbsnbcpy(d2, src, 0);
    CHECK_EQ_U(d2[0], 0xFF); CHECK_EQ_U(d2[1], 0xFF);
}
TC_CASE(extent)
{
    unsigned char src[] = {0x82,0xA0,'a',0}, dst[16];
    fill(dst, 16);
    _mbsncpy(dst, src, 5);
    CHECK_EQ_U(dst[3], 0); CHECK_EQ_U(dst[4], 0); CHECK_EQ_U(dst[5], 0);
    CHECK_EQ_U(dst[6], 0xFF);
}
int main(void) { tc_begin(); old_cp = _getmbcp(); TC_RUN(cp); TC_RUN(zero_count); TC_RUN(extent); _setmbcp(old_cp); return tc_end(); }

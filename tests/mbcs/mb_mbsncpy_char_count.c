/* TC
 * id:    mb_mbsncpy_char_count
 * area:  mbcs
 * funcs: _mbsncpy _setmbcp _getmbcp
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _mbsncpy, _mbsncpy_l
 * note:  count is in characters. No terminator is added when count <= source length; when count exceeds the
 *        source length the destination is NUL padded (only the first pad byte is asserted here; the extent and count==0 are probe tests).
 */
#include "tc.h"
#include <mbstring.h>

static int old_cp;
static void fill(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i] = 0xFF; }
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(two_chars)
{
    unsigned char src[] = {0x82,0xA0,0x82,0xA2,0x82,0xA4,0}, dst[16];
    fill(dst, (int)sizeof dst);
    unsigned char *r = _mbsncpy(dst, src, 2);
    CHECK_EQ_P(r, dst);
    CHECK_MEM(dst, src, 4);
    CHECK_EQ_U(dst[4], 0xFF);
}
TC_CASE(pad)
{
    unsigned char src[] = {0x82,0xA0,'a',0}, dst[16];
    fill(dst, (int)sizeof dst);
    _mbsncpy(dst, src, 5);
    CHECK_MEM(dst, src, 3);
    CHECK_EQ_U(dst[3], 0);
}
int main(void)
{
    tc_begin(); old_cp = _getmbcp();
    TC_RUN(cp); TC_RUN(two_chars); TC_RUN(pad);
    _setmbcp(old_cp);
    return tc_end();
}

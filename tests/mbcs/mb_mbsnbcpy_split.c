/* TC
 * id:    mb_mbsnbcpy_split
 * area:  mbcs
 * funcs: _mbsnbcpy _setmbcp _getmbcp
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _mbsnbcpy: if the last byte copied is a lead byte it is replaced by a null
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static int old_cp;
static void fill(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i] = 0xFF; }
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(whole)
{
    unsigned char src[] = {0x82,0xA0,0x82,0xA2,0}, dst[16];
    fill(dst, 16);
    _mbsnbcpy(dst, src, 4);
    CHECK_MEM(dst, src, 4);
}
TC_CASE(split)
{
    unsigned char src[] = {0x82,0xA0,0x82,0xA2,0}, dst[16];
    fill(dst, 16);
    _mbsnbcpy(dst, src, 3);
    CHECK_EQ_U(dst[0], 0x82); CHECK_EQ_U(dst[1], 0xA0); CHECK_EQ_U(dst[2], 0);
}
TC_CASE(one)
{
    unsigned char src[] = {0x82,0xA0,0x82,0xA2,0}, dst[16];
    fill(dst, 16);
    _mbsnbcpy(dst, src, 1);
    CHECK_EQ_U(dst[0], 0);
}
int main(void) { tc_begin(); old_cp = _getmbcp(); TC_RUN(cp); TC_RUN(whole); TC_RUN(split); TC_RUN(one); _setmbcp(old_cp); return tc_end(); }

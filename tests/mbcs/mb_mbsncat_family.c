/* TC
 * id:    mb_mbsncat_family
 * area:  mbcs
 * funcs: _mbsncat _mbsnbcat _setmbcp _getmbcp
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _mbsncat, _mbsnbcat: result is always NUL terminated
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static int old_cp;
static void fill(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i] = 0xFF; }
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(chars)
{
    unsigned char dst[16] = {'a',0}, src[] = {'x','y','z',0};
    _mbsncat(dst, src, 2);
    CHECK_STR((char*)dst, "axy");
}
TC_CASE(chars_dbcs)
{
    unsigned char dst[16] = {'a',0}, src[] = {0x82,0xA0,0x82,0xA2,0x82,0xA4,0}, e[] = {'a',0x82,0xA0,0x82,0xA2,0};
    _mbsncat(dst, src, 2);
    CHECK_MEM(dst, e, sizeof e);
}
TC_CASE(count_large)
{
    unsigned char dst[16] = {'a',0}, d2[16] = {'a',0}, src[] = {'x','y',0};
    _mbsncat(dst, src, 10);
    CHECK_STR((char*)dst, "axy");
    _mbsnbcat(d2, src, 10);
    CHECK_STR((char*)d2, "axy");
}
int main(void) { tc_begin(); old_cp = _getmbcp(); TC_RUN(cp); TC_RUN(chars); TC_RUN(chars_dbcs); TC_RUN(count_large); _setmbcp(old_cp); return tc_end(); }

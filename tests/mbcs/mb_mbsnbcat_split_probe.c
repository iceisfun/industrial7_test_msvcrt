/* TC
 * id:    mb_mbsnbcat_split_probe
 * area:  mbcs
 * funcs: _mbsnbcat _setmbcp _getmbcp
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _mbsnbcat
 * note:  PROBE: when nbytes would split a DBCS char, the trailing lead byte is not appended and the result is NUL terminated.
 */
#include "tc.h"
#include <mbstring.h>
static int old_cp;
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(bytes_split)
{
    unsigned char dst[16] = {'a',0}, src[] = {0x82,0xA0,0x82,0xA2,0}, e[] = {'a',0x82,0xA0,0};
    _mbsnbcat(dst, src, 3);
    CHECK_MEM(dst, e, sizeof e);
    dst[1] = 0;
    _mbsnbcat(dst, src, 1);
    CHECK_EQ_U(dst[0], 'a'); CHECK_EQ_U(dst[1], 0);
}
int main(void) { tc_begin(); old_cp = _getmbcp(); TC_RUN(cp); TC_RUN(bytes_split); _setmbcp(old_cp); return tc_end(); }

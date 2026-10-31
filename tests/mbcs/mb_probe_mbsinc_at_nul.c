/* TC
 * id:    mb_probe_mbsinc_at_nul
 * area:  mbcs
 * funcs: _mbsinc _setmbcp _getmbcp
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   none (MS docs _mbsinc do not describe the terminator)
 * note:  PROBE at cp932: _mbsinc on the NUL terminator returns the next byte address (s + 1), i.e. it steps one byte
 *        past the terminator like _mbclen(NUL) == 1. Only the returned pointer value is compared, nothing is read.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static int old_cp;
static const unsigned char *volatile v_s;

TC_CASE(mbsinc_nul_probe)
{
    static const unsigned char s[] = {0, 0};
    v_s = s;
    CHECK(_mbsinc(v_s) == s + 1);
}
int main(void)
{
    tc_begin(); old_cp = _getmbcp(); _setmbcp(932);
    TC_RUN(mbsinc_nul_probe);
    _setmbcp(old_cp);
    return tc_end();
}

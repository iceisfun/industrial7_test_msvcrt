/* TC
 * id:    mb_probe_mbbtombc_out_of_range
 * area:  mbcs
 * funcs: _mbbtombc _setmbcp _getmbcp
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   none (argument is a single byte; values above 0xFF are not specified)
 * note:  PROBE at cp932: _mbbtombc(0x100) returns 0x100 unchanged (guess: the conversion table only applies to
 *        0x20-0x7E and 0xA1-0xDF, anything else is returned as is, not truncated to the low byte).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static int old_cp;
static volatile unsigned int v;

TC_CASE(mbbtombc_0x100_probe) { v = 0x100; CHECK_EQ_U(_mbbtombc(v), 0x100); }
int main(void)
{
    tc_begin(); old_cp = _getmbcp(); _setmbcp(932);
    TC_RUN(mbbtombc_0x100_probe);
    _setmbcp(old_cp);
    return tc_end();
}

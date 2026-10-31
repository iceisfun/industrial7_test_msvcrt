/* TC
 * id:    mb_probe_ismbb_high_bits
 * area:  mbcs
 * funcs: _ismbblead _ismbbtrail _ismbbalpha _setmbcp _getmbcp
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   none (undocumented)
 * note:  PROBE: do _ismbb* mask the argument to its low byte or index with the high bits? Guess: low byte is used, so
 *        _ismbblead(0x181) == _ismbblead(0x81) (nonzero at cp932), _ismbbtrail(0x140) nonzero, _ismbbalpha(0x141) nonzero.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static int old_cp;
static volatile unsigned int v;

TC_CASE(lead_0x181_vs_0x81_probe) { v = 0x181; CHECK(_ismbblead(v) != 0); }
TC_CASE(trail_0x140_probe) { v = 0x140; CHECK(_ismbbtrail(v) != 0); }
TC_CASE(alpha_0x141_probe) { v = 0x141; CHECK(_ismbbalpha(v) != 0); }
int main(void)
{
    tc_begin(); old_cp = _getmbcp(); _setmbcp(932);
    TC_RUN(lead_0x181_vs_0x81_probe); TC_RUN(trail_0x140_probe); TC_RUN(alpha_0x141_probe);
    _setmbcp(old_cp);
    return tc_end();
}

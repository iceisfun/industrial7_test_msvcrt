/* TC
 * id:    mb_probe_ismbbtrail_936_7f
 * area:  mbcs
 * funcs: _ismbbtrail _setmbcp
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _ismbbtrail; GBK (cp936) trail range
 * note:  PROBE: 0x7F is NOT a trail byte in cp936 (GBK trail range is 0x40-0x7E,0x80-0xFE). Real Windows decides.
 *        Observed Win10 22H2 ucrtbase: after _setmbcp(936), _ismbbtrail(0x7F) is nonzero (the ucrt 936 table marks 0x40..0xFE as trail,
 *        including 0x7F). Stable (static table).
 */
#include "tc.h"
#include <mbctype.h>

static volatile int v_cp = 936;

TC_CASE(del_not_trail) { CHECK_EQ_I(_setmbcp(v_cp), 0); CHECK(_ismbbtrail(0x7F)); }   /* name kept; observed: 0x7F IS a trail byte in ucrt cp936 */

int main(void) { tc_begin(); TC_RUN(del_not_trail); _setmbcp(_MB_CP_ANSI); return tc_end(); }

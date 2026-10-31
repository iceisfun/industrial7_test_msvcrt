/* TC
 * id:    mb_probe_ismbbtrail_936_7f
 * area:  mbcs
 * funcs: _ismbbtrail _setmbcp
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _ismbbtrail; GBK (cp936) trail range
 * note:  PROBE: 0x7F is NOT a trail byte in cp936 (GBK trail range is 0x40-0x7E,0x80-0xFE). Real Windows decides.
 */
#include "tc.h"
#include <mbctype.h>

static volatile int v_cp = 936;

TC_CASE(del_not_trail) { CHECK_EQ_I(_setmbcp(v_cp), 0); CHECK(!_ismbbtrail(0x7F)); }

int main(void) { tc_begin(); TC_RUN(del_not_trail); _setmbcp(_MB_CP_ANSI); return tc_end(); }

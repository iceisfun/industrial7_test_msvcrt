/* TC
 * id:    mb_ismbbtrail_936
 * area:  mbcs
 * funcs: _ismbbtrail _setmbcp
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _ismbbtrail (exact cp936 trail ranges are not given in the docs)
 * note:  0x7F is deliberately not asserted (see mb_probe_ismbbtrail_936_7f); 0x40-0x7E,0x80-0xFE known from the CRT table.
 */
#include "tc.h"
#include <mbctype.h>

static volatile int v_cp = 936;

TC_CASE(true_bytes) { CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK(_ismbbtrail(0x40)); CHECK(_ismbbtrail(0x7E)); CHECK(_ismbbtrail(0x80)); CHECK(_ismbbtrail(0xFE)); }
TC_CASE(false_bytes) { CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK(!_ismbbtrail(0x3F)); CHECK(!_ismbbtrail(0xFF)); }

int main(void) { tc_begin(); TC_RUN(true_bytes); TC_RUN(false_bytes); _setmbcp(_MB_CP_ANSI); return tc_end(); }

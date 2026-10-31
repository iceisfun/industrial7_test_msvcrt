/* TC
 * id:    mb_ismbbtrail_932
 * area:  mbcs
 * funcs: _ismbbtrail _setmbcp
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _ismbbtrail (exact cp932 trail ranges are not given in the docs)
 * note:  Ranges 0x40-0x7E,0x80-0xFC are the known CRT cp932 table, not spelled out in MS docs.
 */
#include "tc.h"
#include <mbctype.h>

static volatile int v_cp = 932;

TC_CASE(true_bytes) { CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK(_ismbbtrail(0x40)); CHECK(_ismbbtrail(0x7E)); CHECK(_ismbbtrail(0x80)); CHECK(_ismbbtrail(0xFC)); }
TC_CASE(false_bytes) { CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK(!_ismbbtrail(0x3F)); CHECK(!_ismbbtrail(0x7F)); CHECK(!_ismbbtrail(0xFD));
    CHECK(!_ismbbtrail(0x00)); CHECK(!_ismbbtrail(0xFF)); }
TC_CASE(scan) { int c, bad = 0; CHECK_EQ_I(_setmbcp(v_cp), 0);
    for (c = 0; c < 256; c++) {
        int want = (c >= 0x40 && c <= 0x7E) || (c >= 0x80 && c <= 0xFC);
        if ((_ismbbtrail((unsigned int)c) != 0) != want) bad++;
    }
    CHECK_EQ_I(bad, 0); }

int main(void) { tc_begin(); TC_RUN(true_bytes); TC_RUN(false_bytes); TC_RUN(scan); _setmbcp(_MB_CP_ANSI); return tc_end(); }

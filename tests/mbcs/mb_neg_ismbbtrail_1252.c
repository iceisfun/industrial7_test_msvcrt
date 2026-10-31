/* TC
 * id:    mb_neg_ismbbtrail_1252
 * area:  mbcs
 * funcs: _ismbbtrail _setmbcp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _ismbbtrail: single-byte code pages have no trail bytes
 */
#include "tc.h"
#include <mbctype.h>

static volatile int v_cp;

TC_CASE(cp1252) { int c, bad = 0; v_cp = 1252; CHECK_EQ_I(_setmbcp(v_cp), 0);
    for (c = 0; c < 256; c++) if (_ismbbtrail((unsigned int)c)) bad++;
    CHECK_EQ_I(bad, 0); }
TC_CASE(sbcs) { int c, bad = 0; v_cp = _MB_CP_SBCS; CHECK_EQ_I(_setmbcp(v_cp), 0);
    for (c = 0; c < 256; c++) if (_ismbbtrail((unsigned int)c)) bad++;
    CHECK_EQ_I(bad, 0); }

int main(void) { tc_begin(); TC_RUN(cp1252); TC_RUN(sbcs); _setmbcp(_MB_CP_ANSI); return tc_end(); }

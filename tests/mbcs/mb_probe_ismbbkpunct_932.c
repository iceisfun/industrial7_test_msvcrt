/* TC
 * id:    mb_probe_ismbbkpunct_932
 * area:  mbcs
 * funcs: _ismbbkpunct _ismbbkalnum _setmbcp
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _ismbbkpunct, _ismbbkalnum
 * note:  PROBE: in cp932 half-width katakana punctuation is 0xA1-0xA5 (_ismbbkpunct true, _ismbbkalnum false) and
 *        0xB1 is alnum, not punct. This split is the classic JIS X 0201 division; real Windows decides.
 */
#include "tc.h"
#include <mbctype.h>

static volatile int v_cp = 932;

TC_CASE(punct) { CHECK_EQ_I(_setmbcp(v_cp), 0); CHECK(_ismbbkpunct(0xA1)); CHECK(!_ismbbkalnum(0xA1)); }
TC_CASE(alnum_not_punct) { CHECK_EQ_I(_setmbcp(v_cp), 0); CHECK(!_ismbbkpunct(0xB1)); }

int main(void) { tc_begin(); TC_RUN(punct); TC_RUN(alnum_not_punct); _setmbcp(_MB_CP_ANSI); return tc_end(); }

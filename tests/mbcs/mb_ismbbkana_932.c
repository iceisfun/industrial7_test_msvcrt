/* TC
 * id:    mb_ismbbkana_932
 * area:  mbcs
 * funcs: _ismbbkana _ismbbkalnum _setmbcp
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _ismbbkana, _ismbbkalnum (cp932 katakana 0xA1-0xDF)
 */
#include "tc.h"
#include <mbctype.h>

static volatile int v_cp = 932;

TC_CASE(kana_true) { CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK(_ismbbkana(0xB1)); CHECK(_ismbbkana(0xA1)); CHECK(_ismbbkana(0xDF)); }
TC_CASE(kana_false) { CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK(!_ismbbkana(0xA0)); CHECK(!_ismbbkana(0xE0)); CHECK(!_ismbbkana(0x41)); }
TC_CASE(alnum) { CHECK_EQ_I(_setmbcp(v_cp), 0); CHECK(_ismbbkalnum(0xB1)); }

int main(void) { tc_begin(); TC_RUN(kana_true); TC_RUN(kana_false); TC_RUN(alnum); _setmbcp(_MB_CP_ANSI); return tc_end(); }

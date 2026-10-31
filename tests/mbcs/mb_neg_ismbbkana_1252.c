/* TC
 * id:    mb_neg_ismbbkana_1252
 * area:  mbcs
 * funcs: _ismbbkana _setmbcp
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _ismbbkana: katakana only in Japanese code page
 */
#include "tc.h"
#include <mbctype.h>

static volatile int v_cp = 1252;

TC_CASE(none) { CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK_EQ_I(_ismbbkana(0xB1), 0); CHECK_EQ_I(_ismbbkana(0xA1), 0); CHECK_EQ_I(_ismbbkana(0xDF), 0); }

int main(void) { tc_begin(); TC_RUN(none); _setmbcp(_MB_CP_ANSI); return tc_end(); }

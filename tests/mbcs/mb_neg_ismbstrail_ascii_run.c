/* TC
 * id:    mb_neg_ismbstrail_ascii_run
 * area:  mbcs
 * funcs: _ismbstrail _ismbslead _setmbcp
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _ismbstrail: trail only when preceded by a lead in the string
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile int v_cp = 932;

TC_CASE(ascii_pair) { static const unsigned char s[] = { 0x40, 0x40, 0 };
    CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK_EQ_I(_ismbstrail(s, s), 0); CHECK_EQ_I(_ismbstrail(s, s + 1), 0); }
TC_CASE(third_byte) { static const unsigned char s[] = { 0x82, 0x40, 0x40, 0 };
    CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK(_ismbstrail(s, s + 1) != 0); CHECK_EQ_I(_ismbstrail(s, s + 2), 0); CHECK_EQ_I(_ismbslead(s, s + 2), 0); }
TC_CASE(two_chars) { static const unsigned char s[] = { 0x82, 0xA0, 0x82, 0xA0, 0 };
    CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK(_ismbslead(s, s + 2) != 0); CHECK(_ismbstrail(s, s + 3) != 0);
    CHECK_EQ_I(_ismbstrail(s, s + 2), 0); CHECK_EQ_I(_ismbslead(s, s + 3), 0); }

int main(void) { tc_begin(); TC_RUN(ascii_pair); TC_RUN(third_byte); TC_RUN(two_chars); _setmbcp(_MB_CP_ANSI); return tc_end(); }

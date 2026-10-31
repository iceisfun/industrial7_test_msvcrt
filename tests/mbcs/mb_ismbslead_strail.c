/* TC
 * id:    mb_ismbslead_strail
 * area:  mbcs
 * funcs: _ismbslead _ismbstrail _setmbcp
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _ismbslead, _ismbstrail
 * note:  Uses the start pointer so context determines lead vs trail.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile int v_cp = 932;

TC_CASE(lead) { static const unsigned char s[] = { 0x82, 0xA0, 0x41, 0 };
    CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK(_ismbslead(s, s) != 0); CHECK_EQ_I(_ismbslead(s, s + 1), 0); CHECK_EQ_I(_ismbslead(s, s + 2), 0); }
TC_CASE(trail) { static const unsigned char s[] = { 0x82, 0xA0, 0x41, 0 };
    CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK(_ismbstrail(s, s + 1) != 0); CHECK_EQ_I(_ismbstrail(s, s), 0); }
TC_CASE(after_ascii) { static const unsigned char s[] = { 0x41, 0x40, 0 };
    CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK_EQ_I(_ismbstrail(s, s + 1), 0); CHECK_EQ_I(_ismbslead(s, s + 1), 0); }

int main(void) { tc_begin(); TC_RUN(lead); TC_RUN(trail); TC_RUN(after_ascii); _setmbcp(_MB_CP_ANSI); return tc_end(); }

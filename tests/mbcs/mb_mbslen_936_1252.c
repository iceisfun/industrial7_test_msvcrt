/* TC
 * id:    mb_mbslen_936_1252
 * area:  mbcs
 * funcs: _mbslen _setmbcp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mbslen, _setmbcp
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile int v_cp;
static const unsigned char s[] = { 0xB0, 0xA1, 0xB0, 0xA2, 0 };

TC_CASE(cp936) { v_cp = 936; CHECK_EQ_I(_setmbcp(v_cp), 0); CHECK_EQ_U(_mbslen(s), 2); }
TC_CASE(cp1252) { v_cp = 1252; CHECK_EQ_I(_setmbcp(v_cp), 0); CHECK_EQ_U(_mbslen(s), 4); }
TC_CASE(sbcs) { v_cp = _MB_CP_SBCS; CHECK_EQ_I(_setmbcp(v_cp), 0); CHECK_EQ_U(_mbslen(s), 4); }

int main(void) { tc_begin(); TC_RUN(cp936); TC_RUN(cp1252); TC_RUN(sbcs); _setmbcp(_MB_CP_ANSI); return tc_end(); }

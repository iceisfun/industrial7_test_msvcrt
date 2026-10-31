/* TC
 * id:    mb_neg_mbbtype_illegal
 * area:  mbcs
 * funcs: _mbbtype _mbsbtype _setmbcp
 * kind:  negative
 * crt:   all
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _mbbtype (_MBC_ILLEGAL -1)
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;

TC_CASE(bad_trail) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x20; CHECK_EQ_I(_mbbtype(v, 1), -1); v = 0x00; CHECK_EQ_I(_mbbtype(v, 1), -1); v = 0x7F; CHECK_EQ_I(_mbbtype(v, 1), -1); }
TC_CASE(bad_string) { static const unsigned char s[] = { 0x82, 0x20, 0 }; CHECK_EQ_U(_setmbcp(932), 0); CHECK_EQ_I(_mbsbtype(s, 1), -1); }

int main(void) { tc_begin(); TC_RUN(bad_trail); TC_RUN(bad_string); _setmbcp(-3); return tc_end(); }

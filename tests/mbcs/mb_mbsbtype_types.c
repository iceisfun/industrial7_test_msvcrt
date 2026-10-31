/* TC
 * id:    mb_mbsbtype_types
 * area:  mbcs
 * funcs: _mbsbtype _mbbtype _setmbcp
 * crt:   all
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _mbsbtype, _mbbtype (_MBC_SINGLE 0, _MBC_LEAD 1, _MBC_TRAIL 2)
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;

TC_CASE(string) {
    static const unsigned char s[] = { 0x82, 0xA0, 'a', 0 };
    CHECK_EQ_U(_setmbcp(932), 0);
    CHECK_EQ_I(_mbsbtype(s, 0), 1);
    CHECK_EQ_I(_mbsbtype(s, 1), 2);
    CHECK_EQ_I(_mbsbtype(s, 2), 0);
}
TC_CASE(bytes) {
    CHECK_EQ_U(_setmbcp(932), 0);
    v = 0x82; CHECK_EQ_I(_mbbtype(v, 0), 1);
    v = 0xA0; CHECK_EQ_I(_mbbtype(v, 1), 2);
    v = 'a';  CHECK_EQ_I(_mbbtype(v, 0), 0);
    v = 0x41; CHECK_EQ_I(_mbbtype(v, 1), 2);  /* 0x40-0x7E are valid trail bytes in 932 */
}

int main(void) { tc_begin(); TC_RUN(string); TC_RUN(bytes); _setmbcp(-3); return tc_end(); }

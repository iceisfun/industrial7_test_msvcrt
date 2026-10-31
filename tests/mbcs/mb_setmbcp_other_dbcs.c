/* TC
 * id:    mb_setmbcp_other_dbcs
 * area:  mbcs
 * funcs: _setmbcp _getmbcp _ismbblead _ismbbtrail
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _ismbblead, _ismbbtrail
 * note:  Only bounds inside documented-by-codepage ranges: lead 0x81..0xFE, trail 0x40-0x7E / 0xA1-0xFE.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <errno.h>

static volatile int v_cp, v_c;
static void setcp(int cp) { v_cp = cp; CHECK_EQ_I(_setmbcp(v_cp), 0); }
TC_CASE(cp950)
{
    int s = _getmbcp(); setcp(950);
    v_c = 0x81; CHECK(_ismbblead(v_c)); v_c = 0xA1; CHECK(_ismbblead(v_c)); v_c = 0xFE; CHECK(_ismbblead(v_c));
    v_c = 0x80; CHECK(!_ismbblead(v_c)); v_c = 0xFF; CHECK(!_ismbblead(v_c)); v_c = 0x41; CHECK(!_ismbblead(v_c));
    v_c = 0x40; CHECK(_ismbbtrail(v_c)); v_c = 0x7E; CHECK(_ismbbtrail(v_c));
    v_c = 0xA1; CHECK(_ismbbtrail(v_c)); v_c = 0xFE; CHECK(_ismbbtrail(v_c));
    v_c = 0x3F; CHECK(!_ismbbtrail(v_c)); v_c = 0x00; CHECK(!_ismbbtrail(v_c));
    _setmbcp(s);
}
TC_CASE(cp949)
{
    int s = _getmbcp(), c; setcp(949);
    for (c = 0x81; c <= 0xFE; c++) { v_c = c; CHECK(_ismbblead(v_c)); }
    v_c = 0x80; CHECK(!_ismbblead(v_c)); v_c = 0xFF; CHECK(!_ismbblead(v_c));
    v_c = 0x41; CHECK(_ismbbtrail(v_c)); v_c = 0xA1; CHECK(_ismbbtrail(v_c));
    v_c = 0x00; CHECK(!_ismbbtrail(v_c));
    _setmbcp(s);
}

int main(void) { tc_begin(); TC_RUN(cp950); TC_RUN(cp949); return tc_end(); }

/* TC
 * id:    mb_ismbblead_936
 * area:  mbcs
 * funcs: _ismbblead _setmbcp _getmbcp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _ismbblead; GBK lead range 0x81-0xFE
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <errno.h>

static volatile unsigned v_c;
static int lead(unsigned c) { v_c = c; return _ismbblead(v_c) != 0; }
TC_CASE(spot)
{
    int s = _getmbcp(); CHECK_EQ_I(_setmbcp(936), 0);
    CHECK(lead(0x81)); CHECK(lead(0xA1)); CHECK(lead(0xFE));
    CHECK(!lead(0x80)); CHECK(!lead(0xFF)); CHECK(!lead(0x7F));
    _setmbcp(s);
}
TC_CASE(full_scan)
{
    unsigned c; int bad = 0, s = _getmbcp(); CHECK_EQ_I(_setmbcp(936), 0);
    for (c = 0; c < 256; c++)
        if ((lead(c) != 0) != (c >= 0x81 && c <= 0xFE)) bad++;
    CHECK_EQ_I(bad, 0);
    _setmbcp(s);
}

int main(void) { tc_begin(); TC_RUN(spot); TC_RUN(full_scan); return tc_end(); }

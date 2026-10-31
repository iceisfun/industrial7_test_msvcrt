/* TC
 * id:    mb_ismbblead_932
 * area:  mbcs
 * funcs: _ismbblead _setmbcp _getmbcp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _ismbblead; Shift-JIS lead ranges 0x81-0x9F, 0xE0-0xFC
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <errno.h>

static volatile unsigned v_c;
static int lead(unsigned c) { v_c = c; return _ismbblead(v_c) != 0; }
TC_CASE(true_bytes) { int s = _getmbcp(); CHECK_EQ_I(_setmbcp(932), 0); CHECK(lead(0x81)); CHECK(lead(0x9F)); CHECK(lead(0xE0)); CHECK(lead(0xFC)); _setmbcp(s); }
TC_CASE(false_bytes)
{
    int s = _getmbcp(); CHECK_EQ_I(_setmbcp(932), 0);
    CHECK(!lead(0x80)); CHECK(!lead(0xA0)); CHECK(!lead(0xDF)); CHECK(!lead(0xFD)); CHECK(!lead(0xFF));
    CHECK(!lead(0x00)); CHECK(!lead(0x41));
    _setmbcp(s);
}
TC_CASE(full_scan)
{
    unsigned c; int bad = 0, s = _getmbcp(); CHECK_EQ_I(_setmbcp(932), 0);
    for (c = 0; c < 256; c++) {
        int exp = (c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xFC);
        if ((lead(c) != 0) != exp) bad++;
    }
    CHECK_EQ_I(bad, 0);
    _setmbcp(s);
}

int main(void) { tc_begin(); TC_RUN(true_bytes); TC_RUN(false_bytes); TC_RUN(full_scan); return tc_end(); }

/* TC
 * id:    gmc_neg_blank_not_blank
 * area:  mbcs
 * funcs: _ismbcblank _ismbcblank_l _setmbcp
 * procs: 
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _ismbcblank (VS2013+): space or horizontal tab
 * note:  Control and other whitespace (LF VT FF CR), NUL, 0x1F, fullwidth letter 0x8260 are not blank.
 *        Observed Win10 22H2 ucrtbase in CP932: _ismbcblank/_ismbcblank_l are true for 0x09, 0x20 AND 0xA1..0xA5 (halfwidth katakana
 *        punctuation; the ucrt CP932 table has the _BLANK bit there) but false for 0xA0 and 0xA6; in CP1252/ANSI only 0x09 and 0x20.
 *        The negative sweep excludes 0xA1..0xA5, which are asserted TRUE by case halfwidth_blank (observed, deviates from the MS docs
 *        space-or-tab statement). Stable (static table).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int b(unsigned c) { v = c; return _ismbcblank(v) != 0; }
static int bl(unsigned c) { v = c; return _ismbcblank_l(v, NULL) != 0; }
TC_CASE(other_whitespace)
{
    unsigned c;
    CHECK_EQ_I(_setmbcp(932), 0);
    for (c = 0x0A; c <= 0x0D; c++) { CHECK(!b(c)); CHECK(!bl(c)); }
}
TC_CASE(ctrl)
{
    CHECK_EQ_I(_setmbcp(932), 0);
    CHECK(!b(0x00)); CHECK(!b(0x1F));
    CHECK(!bl(0x00)); CHECK(!bl(0x1F));
}
TC_CASE(fullwidth) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(!b(0x8260)); CHECK(!bl(0x8260)); }
TC_CASE(halfwidth_blank)
{
    unsigned c;
    CHECK_EQ_I(_setmbcp(932), 0);
    for (c = 0xA1; c <= 0xA5; c++) { CHECK(b(c)); CHECK(bl(c)); }   /* observed: blank in CP932 */
    CHECK(!b(0xA0)); CHECK(!bl(0xA0)); CHECK(!b(0xA6)); CHECK(!bl(0xA6));
}
TC_CASE(no_iph)
{
    unsigned c;
    CHECK_EQ_I(_setmbcp(932), 0); tc_iph_count = 0;
    for (c = 0; c < 0x100; c++) { if (c != 0x20 && c != 0x09 && !(c >= 0xA1 && c <= 0xA5)) { CHECK(!b(c)); CHECK(!bl(c)); } }
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(other_whitespace); TC_RUN(ctrl); TC_RUN(fullwidth); TC_RUN(halfwidth_blank); TC_RUN(no_iph);
    _setmbcp(-3);
    return tc_end();
}

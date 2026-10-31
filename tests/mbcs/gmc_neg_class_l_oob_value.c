/* TC
 * id:    gmc_neg_class_l_oob_value
 * area:  mbcs
 * funcs: _ismbcspace_l _ismbcalpha_l _ismbcdigit_l _ismbcprint_l _setmbcp
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _ismbcalpha etc. (argument is unsigned int c, documented only as a character code)
 * note:  PROBE: behaviour for c > 0xFFFF. Asserted: no crash, no invalid-parameter handler call, and 0xFFFFFFFF / 0x80000000 classify false (low word 0xFFFF / 0x0000 are not valid characters). Whether the high bits are masked (0x10041 == 'A' result) is only recorded as a non-asserting case.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
typedef int (__cdecl *clsf)(unsigned int, _locale_t);
static int t(clsf f, unsigned c) { v = c; return f(v, NULL) != 0; }
static void all_false(unsigned c)
{
    CHECK(!t(_ismbcspace_l, c)); CHECK(!t(_ismbcalpha_l, c));
    CHECK(!t(_ismbcdigit_l, c)); CHECK(!t(_ismbcprint_l, c));
}
TC_CASE(ffffffff) { CHECK_EQ_I(_setmbcp(932), 0); tc_iph_count = 0; all_false(0xFFFFFFFFu); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(topbit) { CHECK_EQ_I(_setmbcp(932), 0); tc_iph_count = 0; all_false(0x80000000u); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(high_word_no_crash)
{
    volatile int r;
    CHECK_EQ_I(_setmbcp(932), 0); tc_iph_count = 0;
    r = t(_ismbcalpha_l, 0x10041u); r = t(_ismbcdigit_l, 0x10031u); r = t(_ismbcspace_l, 0x10020u); r = t(_ismbcprint_l, 0x10041u);
    (void)r;
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(ffffffff); TC_RUN(topbit); TC_RUN(high_word_no_crash);
    _setmbcp(-3);
    return tc_end();
}

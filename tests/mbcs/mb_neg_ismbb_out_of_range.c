/* TC
 * id:    mb_neg_ismbb_out_of_range
 * area:  mbcs
 * funcs: _ismbblead _ismbbtrail _ismbbalpha _ismbbalnum _ismbbgraph _ismbbprint _ismbbpunct _ismbbkana _setmbcp _getmbcp
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _ismbb* (argument is a byte value 0..255; larger values are not specified)
 * note:  crt limited to the two newest CRTs (older ones may index tables with the unmasked value; not verified).
 *        Out-of-range arguments are not documented, so NOTHING about the result is asserted: this only checks the call
 *        returns without faulting (a crash = failure). Values chosen: 0x100, 0x1081, 0xFFFFFFFF, and a sign-extended char.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static int old_cp;
static volatile unsigned int v_arg;
static volatile int v_sink;

static void all(unsigned int a)
{
    v_arg = a;
    v_sink = _ismbblead(v_arg); v_sink = _ismbbtrail(v_arg); v_sink = _ismbbalpha(v_arg);
    v_sink = _ismbbalnum(v_arg); v_sink = _ismbbgraph(v_arg); v_sink = _ismbbprint(v_arg);
    v_sink = _ismbbpunct(v_arg); v_sink = _ismbbkana(v_arg);
}
TC_CASE(no_crash_0x100) { all(0x100); CHECK(1); }
TC_CASE(no_crash_0x1081) { all(0x1081); CHECK(1); }
TC_CASE(no_crash_uint_max) { all(0xFFFFFFFFu); CHECK(1); }
TC_CASE(negative_int_cast) { volatile char c = (char)0x81; int i = c; all((unsigned int)i); CHECK_EQ_I(i, -127); }
int main(void)
{
    tc_begin(); old_cp = _getmbcp(); _setmbcp(932);
    TC_RUN(no_crash_0x100); TC_RUN(no_crash_0x1081); TC_RUN(no_crash_uint_max); TC_RUN(negative_int_cast);
    _setmbcp(old_cp);
    return tc_end();
}

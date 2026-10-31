/* TC
 * id:    mb_neg_mbctolower_invalid_codes
 * area:  mbcs
 * funcs: _mbctolower _mbctoupper _mbctombb _setmbcp _getmbcp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _mbctolower/_mbctoupper (only letters change), _mbctombb (unconvertible values returned as is)
 * note:  cp932. Codes that are not valid characters (lead + invalid trail) or not letters must be returned unchanged.
 *        _mbbtype is covered by mb_neg_mbbtype_illegal; out-of-range _mbbtombc arg is in mb_probe_mbbtombc_out_of_range.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static int old_cp;
static volatile unsigned int v;

TC_CASE(lower_81_00_unchanged) { v = 0x8100; CHECK_EQ_U(_mbctolower(v), 0x8100); CHECK_EQ_U(_mbctoupper(v), 0x8100); }
TC_CASE(lower_81_ff_unchanged) { v = 0x81FF; CHECK_EQ_U(_mbctolower(v), 0x81FF); CHECK_EQ_U(_mbctoupper(v), 0x81FF); }
TC_CASE(lower_ffff_unchanged) { v = 0xFFFF; CHECK_EQ_U(_mbctolower(v), 0xFFFF); CHECK_EQ_U(_mbctoupper(v), 0xFFFF); }
TC_CASE(upper_nonletter_unchanged)
{
    v = '1'; CHECK_EQ_U(_mbctoupper(v), '1'); CHECK_EQ_U(_mbctolower(v), '1');
    v = 0x8140; CHECK_EQ_U(_mbctoupper(v), 0x8140);  /* ideographic space */
    v = 0x82A0; CHECK_EQ_U(_mbctoupper(v), 0x82A0);  /* hiragana A */
    v = 0xB1;   CHECK_EQ_U(_mbctoupper(v), 0xB1);    /* halfwidth katakana */
}
TC_CASE(mbctombb_0_unchanged) { v = 0; CHECK_EQ_U(_mbctombb(v), 0); }
int main(void)
{
    tc_begin(); old_cp = _getmbcp(); _setmbcp(932);
    TC_RUN(lower_81_00_unchanged); TC_RUN(lower_81_ff_unchanged); TC_RUN(lower_ffff_unchanged);
    TC_RUN(upper_nonletter_unchanged); TC_RUN(mbctombb_0_unchanged);
    _setmbcp(old_cp);
    return tc_end();
}

/* TC
 * id:    ct_neg_isascii_high_values
 * area:  ctype
 * funcs: __isascii
 * procs: isascii iswascii
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs __isascii, isascii, __iswascii (nonzero only for 0x00-0x7F)
 * note:  Only 0x80-0xFF (and wide 0x80/0x100/0xFFFF) asserted; arguments outside 0..255 for the narrow forms live in ct_probe_isascii_out_of_range.
 */
#include "tc.h"
#include <ctype.h>
#include <wctype.h>

static volatile int vc;
static volatile unsigned int vw;
typedef int (__cdecl *isascii_t)(int);
static isascii_t p_isascii, p_iswascii;

TC_CASE(c128_255_zero_for_isascii_and___isascii)
{
    int c, bad = 0;
    if (!p_isascii) TC_SKIP("isascii not exported");
    for (c = 128; c <= 255; c++) { vc = c; if (p_isascii(vc) != 0 || (__isascii)(vc) != 0) bad++; }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(iswascii_0x80_0x100_0xFFFF_zero)
{
    if (!p_iswascii) TC_SKIP("iswascii not exported");
    vw = 0x80; CHECK_EQ_I(p_iswascii(vw), 0);
    vw = 0x100; CHECK_EQ_I(p_iswascii(vw), 0);
    vw = 0xFFFF; CHECK_EQ_I(p_iswascii(vw), 0);
    vw = 0x7F; CHECK(p_iswascii(vw) != 0);
}

int main(void) { tc_begin(); p_isascii = (isascii_t)tc_crt_proc("isascii"); p_iswascii = (isascii_t)tc_crt_proc("iswascii"); TC_RUN(c128_255_zero_for_isascii_and___isascii); TC_RUN(iswascii_0x80_0x100_0xFFFF_zero); return tc_end(); }

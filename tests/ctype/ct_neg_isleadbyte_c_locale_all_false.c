/* TC
 * id:    ct_neg_isleadbyte_c_locale_all_false
 * area:  ctype
 * funcs: isleadbyte
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs isleadbyte (C locale single-byte: no lead bytes)
 */
#include "tc.h"
#include <ctype.h>
#include <stdlib.h>

static volatile int vc;

TC_CASE(c0_255_all_zero) { int c, bad = 0; for (c = 0; c <= 255; c++) { vc = c; if (isleadbyte(vc) != 0) bad++; } CHECK_EQ_I(bad, 0); }
TC_CASE(c0x81_0xE0_0xFC_zero) { vc = 0x81; CHECK_EQ_I(isleadbyte(vc), 0); vc = 0xE0; CHECK_EQ_I(isleadbyte(vc), 0); vc = 0xFC; CHECK_EQ_I(isleadbyte(vc), 0); }
TC_CASE(c0xFF_zero) { vc = 0xFF; CHECK_EQ_I(isleadbyte(vc), 0); }
TC_CASE(MB_CUR_MAX_1_consistency) { CHECK_EQ_I(MB_CUR_MAX, 1); }

int main(void) { tc_begin(); TC_RUN(c0_255_all_zero); TC_RUN(c0x81_0xE0_0xFC_zero); TC_RUN(c0xFF_zero); TC_RUN(MB_CUR_MAX_1_consistency); return tc_end(); }

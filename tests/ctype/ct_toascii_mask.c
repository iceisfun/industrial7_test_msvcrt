/* TC
 * id:    ct_toascii_mask
 * area:  ctype
 * funcs: 
 * procs: __toascii
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs __toascii (clears all but the low 7 bits)
 */
#include "tc.h"
#include <ctype.h>

static volatile int v;
TC_DECL_PROC(int, __toascii, (int));
#define TA(x) (v = (x), tc_p___toascii(v))

TC_CASE(ascii_unchanged) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I(TA(c), c); }
TC_CASE(high_masked) { CHECK_EQ_I(TA(0xE9), 0x69); CHECK_EQ_I(TA(0xFF), 0x7F); CHECK_EQ_I(TA(0x80), 0x00); }
TC_CASE(above_255_masked) { CHECK_EQ_I(TA(0x141), 0x41); }
TC_CASE(negative_masked) { CHECK_EQ_I(TA(-1), 0x7F); }

int main(void) { tc_begin(); TC_LOAD_PROC(__toascii); if (!tc_p___toascii) { tc_write("TC FAIL ct_toascii_mask::__toascii not exported\n"); return 1; } TC_RUN(ascii_unchanged); TC_RUN(high_masked); TC_RUN(above_255_masked); TC_RUN(negative_masked); return tc_end(); }

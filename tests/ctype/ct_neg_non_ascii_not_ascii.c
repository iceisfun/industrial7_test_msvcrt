/* TC
 * id:    ct_neg_non_ascii_not_ascii
 * area:  ctype
 * funcs: 
 * procs: __isascii __toascii iswascii
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs __isascii (nonzero iff c in 0..0x7F), iswascii
 */
#include "tc.h"
#include <ctype.h>
#include <wchar.h>

static volatile int v;
TC_DECL_PROC(int, __isascii, (int));
TC_DECL_PROC(int, __toascii, (int));
TC_DECL_PROC(int, iswascii, (wint_t));

TC_CASE(isascii_rejects_128_up) { int c; for (c = 128; c < 1024; c++) { v = c; CHECK_EQ_I(tc_p___isascii(v), 0); } }
TC_CASE(isascii_rejects_negative) { v = -1; CHECK_EQ_I(tc_p___isascii(v), 0); v = -128; CHECK_EQ_I(tc_p___isascii(v), 0); }
TC_CASE(toascii_never_above_127) { int c; for (c = -300; c < 1024; c++) { v = c; CHECK(((unsigned)tc_p___toascii(v)) <= 0x7F); } }
TC_CASE(iswascii_rejects_non_ascii) { int c; for (c = 128; c < 512; c++) CHECK_EQ_I(tc_p_iswascii((wint_t)c) != 0, 0); }
TC_CASE(isascii_accepts_0_127) { int c; for (c = 0; c < 128; c++) { v = c; CHECK(tc_p___isascii(v) != 0); } }

int main(void) { tc_begin(); TC_LOAD_PROC(__isascii); TC_LOAD_PROC(__toascii); TC_LOAD_PROC(iswascii); if (!tc_p___isascii || !tc_p___toascii || !tc_p_iswascii) { tc_write("TC FAIL ct_neg_non_ascii_not_ascii::export missing\n"); return 1; } TC_RUN(isascii_rejects_128_up); TC_RUN(isascii_rejects_negative); TC_RUN(toascii_never_above_127); TC_RUN(iswascii_rejects_non_ascii); TC_RUN(isascii_accepts_0_127); return tc_end(); }

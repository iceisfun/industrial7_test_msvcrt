/* TC
 * id:    ct_iswascii_basic
 * area:  ctype
 * funcs: 
 * procs: iswascii
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs iswascii
 */
#include "tc.h"
#include <ctype.h>
#include <wchar.h>

static volatile wint_t v;
TC_DECL_PROC(int, iswascii, (wint_t));
#define A(x) (v = (wint_t)(x), tc_p_iswascii(v) != 0)

TC_CASE(zero_to_127_nonzero) { int c; for (c = 0; c < 128; c++) CHECK(A(c)); }
TC_CASE(128_to_0xFFFE_zero_sampled) { int c; for (c = 128; c < 0xFFFF; c += 37) CHECK(!A(c)); CHECK(!A(128)); CHECK(!A(0xFF)); CHECK(!A(0xFFFE)); }
TC_CASE(x100_zero) { CHECK(!A(0x100)); }

int main(void) { tc_begin(); TC_LOAD_PROC(iswascii); if (!tc_p_iswascii) { tc_write("TC FAIL ct_iswascii_basic::iswascii not exported\n"); return 1; } TC_RUN(zero_to_127_nonzero); TC_RUN(128_to_0xFFFE_zero_sampled); TC_RUN(x100_zero); return tc_end(); }

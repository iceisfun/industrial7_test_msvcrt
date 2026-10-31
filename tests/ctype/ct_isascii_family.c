/* TC
 * id:    ct_isascii_family
 * area:  ctype
 * funcs: __isascii
 * procs: isascii
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs __isascii, isascii, __iswascii
 * note:  Documented nonzero for 0x00-0x7F. Out-of-range values belong to a separate test.
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;
typedef int (__cdecl *isascii_t)(int);
static isascii_t p_isascii;

TC_CASE(isascii_in_range) { int c, bad = 0; if (!p_isascii) TC_SKIP("isascii not exported"); for (c = 0; c < 128; c++) { vc = c; if (p_isascii(vc) == 0) bad++; } CHECK_EQ_I(bad, 0); }
TC_CASE(dunder_in_range) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if ((__isascii)(vc) == 0) bad++; } CHECK_EQ_I(bad, 0); }
TC_CASE(nul_del) { if (!p_isascii) TC_SKIP("isascii not exported"); vc = 0; CHECK(p_isascii(vc) != 0); CHECK((__isascii)(vc) != 0); vc = 0x7F; CHECK(p_isascii(vc) != 0); CHECK((__isascii)(vc) != 0); }
TC_CASE(agree) { int c, bad = 0; if (!p_isascii) TC_SKIP("isascii not exported"); for (c = 0; c < 128; c++) { vc = c; if ((p_isascii(vc) != 0) != ((__isascii)(vc) != 0)) bad++; } CHECK_EQ_I(bad, 0); }

int main(void) { tc_begin(); p_isascii = (isascii_t)tc_crt_proc("isascii"); TC_RUN(isascii_in_range); TC_RUN(dunder_in_range); TC_RUN(nul_del); TC_RUN(agree); return tc_end(); }

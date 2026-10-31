/* TC
 * id:    ct_isspace_ascii_sweep
 * area:  ctype
 * funcs: isspace
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.4.1.10; MS docs isspace
 * note:  Called as (fn)(c) to bypass mingw macros; c is volatile; only truthiness is asserted.
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;
#define T(f, c) (((f)(c)) != 0)

TC_CASE(positives) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if ((c == 9 || c == 10 || c == 11 || c == 12 || c == 13 || c == 32)) { if (T(isspace, vc) != 1) bad++; } } CHECK_EQ_I(bad, 0); }
TC_CASE(negatives) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (!((c == 9 || c == 10 || c == 11 || c == 12 || c == 13 || c == 32))) { if (T(isspace, vc) != 0) bad++; } } CHECK_EQ_I(bad, 0); }
TC_CASE(nul_false) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (c == 0) { if (T(isspace, vc) != 0) bad++; } } CHECK_EQ_I(bad, 0); }
TC_CASE(fs_gs_rs_us_false) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (c >= 0x1C && c <= 0x1F) { if (T(isspace, vc) != 0) bad++; } } CHECK_EQ_I(bad, 0); }

int main(void) { tc_begin(); TC_RUN(positives); TC_RUN(negatives); TC_RUN(nul_false); TC_RUN(fs_gs_rs_us_false); return tc_end(); }

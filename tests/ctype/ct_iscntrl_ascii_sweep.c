/* TC
 * id:    ct_iscntrl_ascii_sweep
 * area:  ctype
 * funcs: iscntrl
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.4.1.4; MS docs iscntrl
 * note:  Called as (fn)(c) to bypass mingw macros; c is volatile; only truthiness is asserted.
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;
#define T(f, c) (((f)(c)) != 0)

TC_CASE(positives) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if ((c < 0x20 || c == 0x7F)) { if (T(iscntrl, vc) != 1) bad++; } } CHECK_EQ_I(bad, 0); }
TC_CASE(negatives) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (!((c < 0x20 || c == 0x7F))) { if (T(iscntrl, vc) != 0) bad++; } } CHECK_EQ_I(bad, 0); }
TC_CASE(nul_true) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (c == 0) { if (T(iscntrl, vc) != 1) bad++; } } CHECK_EQ_I(bad, 0); }
TC_CASE(del_true) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (c == 0x7F) { if (T(iscntrl, vc) != 1) bad++; } } CHECK_EQ_I(bad, 0); }

int main(void) { tc_begin(); TC_RUN(positives); TC_RUN(negatives); TC_RUN(nul_true); TC_RUN(del_true); return tc_end(); }

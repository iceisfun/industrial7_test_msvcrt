/* TC
 * id:    ct_isdigit_ascii_sweep
 * area:  ctype
 * funcs: isdigit
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.4.1.5; MS docs isdigit
 * note:  Called as (fn)(c) to bypass mingw macros; c is volatile; only truthiness is asserted.
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;
#define T(f, c) (((f)(c)) != 0)

TC_CASE(positives) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if ((c >= '0' && c <= '9')) { if (T(isdigit, vc) != 1) bad++; } } CHECK_EQ_I(bad, 0); }
TC_CASE(negatives) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (!((c >= '0' && c <= '9'))) { if (T(isdigit, vc) != 0) bad++; } } CHECK_EQ_I(bad, 0); }
TC_CASE(nul_false) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (c == 0) { if (T(isdigit, vc) != 0) bad++; } } CHECK_EQ_I(bad, 0); }

int main(void) { tc_begin(); TC_RUN(positives); TC_RUN(negatives); TC_RUN(nul_false); return tc_end(); }

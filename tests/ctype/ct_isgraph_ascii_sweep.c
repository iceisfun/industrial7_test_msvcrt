/* TC
 * id:    ct_isgraph_ascii_sweep
 * area:  ctype
 * funcs: isgraph
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.4.1.6; MS docs isgraph
 * note:  Called as (fn)(c) to bypass mingw macros; c is volatile; only truthiness is asserted.
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;
#define T(f, c) (((f)(c)) != 0)

TC_CASE(positives) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if ((c >= 0x21 && c <= 0x7E)) { if (T(isgraph, vc) != 1) bad++; } } CHECK_EQ_I(bad, 0); }
TC_CASE(negatives) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (!((c >= 0x21 && c <= 0x7E))) { if (T(isgraph, vc) != 0) bad++; } } CHECK_EQ_I(bad, 0); }
TC_CASE(space_false) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (c == ' ') { if (T(isgraph, vc) != 0) bad++; } } CHECK_EQ_I(bad, 0); }

int main(void) { tc_begin(); TC_RUN(positives); TC_RUN(negatives); TC_RUN(space_false); return tc_end(); }

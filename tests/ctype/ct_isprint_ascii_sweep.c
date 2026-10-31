/* TC
 * id:    ct_isprint_ascii_sweep
 * area:  ctype
 * funcs: isprint
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.4.1.7; MS docs isprint
 * note:  Called as (fn)(c) to bypass mingw macros; c is volatile; only truthiness is asserted.
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;
#define T(f, c) (((f)(c)) != 0)

TC_CASE(positives) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if ((c >= 0x20 && c <= 0x7E)) { if (T(isprint, vc) != 1) bad++; } } CHECK_EQ_I(bad, 0); }
TC_CASE(negatives) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (!((c >= 0x20 && c <= 0x7E))) { if (T(isprint, vc) != 0) bad++; } } CHECK_EQ_I(bad, 0); }
TC_CASE(del_false) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (c == 0x7F) { if (T(isprint, vc) != 0) bad++; } } CHECK_EQ_I(bad, 0); }

int main(void) { tc_begin(); TC_RUN(positives); TC_RUN(negatives); TC_RUN(del_false); return tc_end(); }

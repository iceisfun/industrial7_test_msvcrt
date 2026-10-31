/* TC
 * id:    ct_ispunct_ascii_sweep
 * area:  ctype
 * funcs: ispunct
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.4.1.8; MS docs ispunct
 * note:  Called as (fn)(c) to bypass mingw macros; c is volatile; only truthiness is asserted.
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;
#define T(f, c) (((f)(c)) != 0)

TC_CASE(positives) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if ((c > 32 && c < 127 && !(((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) || (c >= '0' && c <= '9')))) { if (T(ispunct, vc) != 1) bad++; } } CHECK_EQ_I(bad, 0); }
TC_CASE(negatives) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (!((c > 32 && c < 127 && !(((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) || (c >= '0' && c <= '9'))))) { if (T(ispunct, vc) != 0) bad++; } } CHECK_EQ_I(bad, 0); }
TC_CASE(del_false) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (c == 0x7F) { if (T(ispunct, vc) != 0) bad++; } } CHECK_EQ_I(bad, 0); }
TC_CASE(space_false) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (c == ' ') { if (T(ispunct, vc) != 0) bad++; } } CHECK_EQ_I(bad, 0); }

int main(void) { tc_begin(); TC_RUN(positives); TC_RUN(negatives); TC_RUN(del_false); TC_RUN(space_false); return tc_end(); }

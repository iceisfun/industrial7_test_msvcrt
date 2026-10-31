/* TC
 * id:    ct_isxdigit_ascii_sweep
 * area:  ctype
 * funcs: isxdigit
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.4.1.12; MS docs isxdigit
 * note:  Called as (fn)(c) to bypass mingw macros; c is volatile; only truthiness is asserted.
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;
#define T(f, c) (((f)(c)) != 0)

TC_CASE(positives) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) { if (T(isxdigit, vc) != 1) bad++; } } CHECK_EQ_I(bad, 0); }
TC_CASE(negatives) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (!(((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')))) { if (T(isxdigit, vc) != 0) bad++; } } CHECK_EQ_I(bad, 0); }
TC_CASE(gz_false) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (c == 'g' || c == 'G' || c == 'z' || c == 'Z') { if (T(isxdigit, vc) != 0) bad++; } } CHECK_EQ_I(bad, 0); }

int main(void) { tc_begin(); TC_RUN(positives); TC_RUN(negatives); TC_RUN(gz_false); return tc_end(); }

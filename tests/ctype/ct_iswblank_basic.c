/* TC
 * id:    ct_iswblank_basic
 * area:  ctype
 * funcs: iswblank
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.30.2.1.3; MS docs isblank
 */
#include "tc.h"
#include <wctype.h>

static volatile wint_t v;

TC_CASE(space_tab_true) { v = L' '; CHECK((iswblank)(v) != 0); v = L'\t'; CHECK((iswblank)(v) != 0); }
TC_CASE(other_ascii_false) { int c; for (c = 0; c < 128; c++) { if (c == ' ' || c == '\t') continue; v = (wint_t)c; CHECK_EQ_I((iswblank)(v) != 0, 0); } }

int main(void) { tc_begin(); TC_RUN(space_tab_true); TC_RUN(other_ascii_false); return tc_end(); }

/* TC
 * id:    ct_isblank_basic
 * area:  ctype
 * funcs: isblank
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.4.1.3; MS docs isblank
 * note:  msvcr100 and earlier do not export isblank.
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;

TC_CASE(space_tab_true) { vc = ' '; CHECK((isblank)(vc) != 0); vc = '\t'; CHECK((isblank)(vc) != 0); }
TC_CASE(other_ws_false) { vc = '\n'; CHECK((isblank)(vc) == 0); vc = '\r'; CHECK((isblank)(vc) == 0); vc = '\v'; CHECK((isblank)(vc) == 0); vc = '\f'; CHECK((isblank)(vc) == 0); }
TC_CASE(letters_digits_false) { vc = 'a'; CHECK((isblank)(vc) == 0); vc = 'Z'; CHECK((isblank)(vc) == 0); vc = '0'; CHECK((isblank)(vc) == 0); vc = 0; CHECK((isblank)(vc) == 0); }
TC_CASE(sweep_only_9_and_32) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (((isblank)(vc) != 0) != (c == 9 || c == 32)) bad++; } CHECK_EQ_I(bad, 0); }

int main(void) { tc_begin(); TC_RUN(space_tab_true); TC_RUN(other_ws_false); TC_RUN(letters_digits_false); TC_RUN(sweep_only_9_and_32); return tc_end(); }

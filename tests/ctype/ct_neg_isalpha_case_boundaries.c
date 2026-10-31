/* TC
 * id:    ct_neg_isalpha_case_boundaries
 * area:  ctype
 * funcs: isalpha isupper islower
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.4.1.2, 7.4.1.11, 7.4.1.7
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;

TC_CASE(at_0x40_lbracket_0x5B_not_alpha) { vc = 0x40; CHECK_EQ_I(isalpha(vc), 0); vc = 0x5B; CHECK_EQ_I(isalpha(vc), 0); vc = 'A'; CHECK(isalpha(vc) != 0); vc = 'Z'; CHECK(isalpha(vc) != 0); }
TC_CASE(backtick_0x60_lbrace_0x7B_not_alpha) { vc = 0x60; CHECK_EQ_I(isalpha(vc), 0); vc = 0x7B; CHECK_EQ_I(isalpha(vc), 0); vc = 'a'; CHECK(isalpha(vc) != 0); vc = 'z'; CHECK(isalpha(vc) != 0); }
TC_CASE(isupper_lowercase_false) { int c, bad = 0; for (c = 'a'; c <= 'z'; c++) { vc = c; if (isupper(vc)) bad++; } CHECK_EQ_I(bad, 0); vc = 0x40; CHECK_EQ_I(isupper(vc), 0); vc = 0x5B; CHECK_EQ_I(isupper(vc), 0); }
TC_CASE(islower_uppercase_false) { int c, bad = 0; for (c = 'A'; c <= 'Z'; c++) { vc = c; if (islower(vc)) bad++; } CHECK_EQ_I(bad, 0); vc = 0x60; CHECK_EQ_I(islower(vc), 0); vc = 0x7B; CHECK_EQ_I(islower(vc), 0); }

int main(void) { tc_begin(); TC_RUN(at_0x40_lbracket_0x5B_not_alpha); TC_RUN(backtick_0x60_lbrace_0x7B_not_alpha); TC_RUN(isupper_lowercase_false); TC_RUN(islower_uppercase_false); return tc_end(); }

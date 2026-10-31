/* TC
 * id:    ct_neg_isdigit_isxdigit_boundaries
 * area:  ctype
 * funcs: isdigit isxdigit
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.4.1.5, 7.4.1.12
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;

TC_CASE(isdigit_slash_0x2F_and_colon_0x3A_false) { vc = 0x2F; CHECK_EQ_I(isdigit(vc), 0); vc = 0x3A; CHECK_EQ_I(isdigit(vc), 0); vc = '0'; CHECK(isdigit(vc) != 0); vc = '9'; CHECK(isdigit(vc) != 0); }
TC_CASE(isxdigit_G_g_false) { vc = 'G'; CHECK_EQ_I(isxdigit(vc), 0); vc = 'g'; CHECK_EQ_I(isxdigit(vc), 0); vc = 'F'; CHECK(isxdigit(vc) != 0); vc = 'f'; CHECK(isxdigit(vc) != 0); }
TC_CASE(isxdigit_at_0x40_and_backtick_0x60_false) { vc = 0x40; CHECK_EQ_I(isxdigit(vc), 0); vc = 0x60; CHECK_EQ_I(isxdigit(vc), 0); vc = 'A'; CHECK(isxdigit(vc) != 0); vc = 'a'; CHECK(isxdigit(vc) != 0); }
TC_CASE(isxdigit_slash_colon_false) { vc = 0x2F; CHECK_EQ_I(isxdigit(vc), 0); vc = 0x3A; CHECK_EQ_I(isxdigit(vc), 0); }

int main(void) { tc_begin(); TC_RUN(isdigit_slash_0x2F_and_colon_0x3A_false); TC_RUN(isxdigit_G_g_false); TC_RUN(isxdigit_at_0x40_and_backtick_0x60_false); TC_RUN(isxdigit_slash_colon_false); return tc_end(); }

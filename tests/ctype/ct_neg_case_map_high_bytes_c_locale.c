/* TC
 * id:    ct_neg_case_map_high_bytes_c_locale
 * area:  ctype
 * funcs: toupper tolower
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.4.2.1, 7.4.2.2
 * note:  In the C locale only A-Z/a-z are case pairs; 0x80-0xFF map to themselves.
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;

TC_CASE(toupper_unchanged_0x80_0xFF) { int c, bad = 0; for (c = 0x80; c <= 0xFF; c++) { vc = c; if (toupper(vc) != c) bad++; } CHECK_EQ_I(bad, 0); }
TC_CASE(tolower_unchanged_0x80_0xFF) { int c, bad = 0; for (c = 0x80; c <= 0xFF; c++) { vc = c; if (tolower(vc) != c) bad++; } CHECK_EQ_I(bad, 0); }
TC_CASE(toupper_0xE9_stays_0xE9) { vc = 0xE9; CHECK_EQ_I(toupper(vc), 0xE9); }
TC_CASE(tolower_0xC9_stays_0xC9) { vc = 0xC9; CHECK_EQ_I(tolower(vc), 0xC9); }

int main(void) { tc_begin(); TC_RUN(toupper_unchanged_0x80_0xFF); TC_RUN(tolower_unchanged_0x80_0xFF); TC_RUN(toupper_0xE9_stays_0xE9); TC_RUN(tolower_0xC9_stays_0xC9); return tc_end(); }

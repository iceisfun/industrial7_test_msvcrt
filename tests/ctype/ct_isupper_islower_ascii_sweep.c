/* TC
 * id:    ct_isupper_islower_ascii_sweep
 * area:  ctype
 * funcs: isupper islower
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.4.1.11, 7.4.1.9; MS docs isupper, islower
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;

TC_CASE(upper) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (((isupper)(vc) != 0) != (c >= 'A' && c <= 'Z')) bad++; } CHECK_EQ_I(bad, 0); }
TC_CASE(lower) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (((islower)(vc) != 0) != (c >= 'a' && c <= 'z')) bad++; } CHECK_EQ_I(bad, 0); }
TC_CASE(disjoint) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if ((isupper)(vc) && (islower)(vc)) bad++; } CHECK_EQ_I(bad, 0); }
TC_CASE(digits_punct_ctrl_neither) { int c, bad = 0; for (c = 0; c < 128; c++) { vc = c; if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) { if ((isupper)(vc) || (islower)(vc)) bad++; } } CHECK_EQ_I(bad, 0); }

int main(void) { tc_begin(); TC_RUN(upper); TC_RUN(lower); TC_RUN(disjoint); TC_RUN(digits_punct_ctrl_neither); return tc_end(); }

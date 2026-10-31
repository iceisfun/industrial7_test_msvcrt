/* TC
 * id:    ct_neg_isclass_high_bytes_c_locale
 * area:  ctype
 * funcs: isalpha isdigit isspace ispunct isprint iscntrl isxdigit isalnum isgraph isupper islower
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.4.1; MS docs isalpha, isprint (C locale)
 * note:  Standard guarantees only that alpha/digit/xdigit/space/upper/lower are false for non-basic chars in the C locale; print/graph/punct/cntrl for 0x80-0xFF are implementation-defined, Windows C locale returns 0.
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;
#define SWEEP(f) do { int c, bad = 0; for (c = 0x80; c <= 0xFF; c++) { vc = c; if (f(vc) != 0) bad++; } CHECK_EQ_I(bad, 0); } while (0)

TC_CASE(alpha_digit_alnum_xdigit_upper_lower_zero)
{ SWEEP(isalpha); SWEEP(isdigit); SWEEP(isalnum); SWEEP(isxdigit); SWEEP(isupper); SWEEP(islower); }
TC_CASE(space_zero) { SWEEP(isspace); }
TC_CASE(ispunct_isgraph_isprint_zero) { SWEEP(ispunct); SWEEP(isgraph); SWEEP(isprint); }
TC_CASE(iscntrl_zero_0x80_0xFF) { SWEEP(iscntrl); }

int main(void) { tc_begin(); TC_RUN(alpha_digit_alnum_xdigit_upper_lower_zero); TC_RUN(space_zero); TC_RUN(ispunct_isgraph_isprint_zero); TC_RUN(iscntrl_zero_0x80_0xFF); return tc_end(); }

/* TC
 * id:    cv_strtol_hex_prefix_only_probe
 * area:  conv
 * funcs: strtol strtoul _strtoi64
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.22.1.4p4-p7 (subject sequence is the initial "0"; endptr points at the 'x')
 * note:  PROBE: "0x" / "0xg" with base 16 (and base 0 "0x"): ISO C says the "0" is converted and endptr == s+1.
 *        Legacy msvcrt strtol skipped the "0x" before checking for digits and may have reported no conversion
 *        (endptr == s); UCRT behaviour unverified. Expected value is the ISO one.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile p;

TC_CASE(base16_0x) { char *e = (char *)1; p = "0x"; CHECK_EQ_I(strtol(p, &e, 16), 0); CHECK_EQ_P(e, p + 1); }
TC_CASE(base16_0xg) { char *e = (char *)1; p = "0xg"; CHECK_EQ_I(strtol(p, &e, 16), 0); CHECK_EQ_P(e, p + 1); }
TC_CASE(base0_0x) { char *e = (char *)1; p = "0x"; CHECK_EQ_I(strtol(p, &e, 0), 0); CHECK_EQ_P(e, p + 1); }
TC_CASE(strtoul_0x) { char *e = (char *)1; p = "0x"; CHECK_EQ_U(strtoul(p, &e, 16), 0); CHECK_EQ_P(e, p + 1); }
TC_CASE(i64_0x) { char *e = (char *)1; p = "0x"; CHECK_EQ_I(_strtoi64(p, &e, 16), 0); CHECK_EQ_P(e, p + 1); }

int main(void) { tc_begin(); TC_RUN(base16_0x); TC_RUN(base16_0xg); TC_RUN(base0_0x); TC_RUN(strtoul_0x); TC_RUN(i64_0x); return tc_end(); }

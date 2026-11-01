/* TC
 * id:    cv_strtol_hex_prefix_only_probe
 * area:  conv
 * funcs: strtol strtoul _strtoi64
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   C11 7.22.1.4p4-p7 (subject sequence is the initial "0"; endptr points at the 'x')
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice): strtol/strtoul/_strtoi64 of "0x" and "0xg" (base 16 and base 0) return 0 with
 *        endptr == nptr (NO characters consumed), not nptr+1 as ISO C says. deviates from C11 7.22.1.4p4 (the "0" before a prefix-only "0x" is not converted; endptr == nptr).
 *        Pre-UCRT branch keeps the ISO expectation (unverified).
 */
#include "tc.h"
#include <stdlib.h>

#if TC_AT_LEAST(140)
#define EXP_END(p) (p)       /* observed Win10 22H2 ucrtbase: "0x" without digits -> no conversion, endptr == nptr */
#else
#define EXP_END(p) ((p) + 1) /* ISO C: only the "0" converted (unverified on pre-UCRT CRTs) */
#endif
static const char *volatile p;

TC_CASE(base16_0x) { char *e = (char *)1; p = "0x"; CHECK_EQ_I(strtol(p, &e, 16), 0); CHECK_EQ_P(e, EXP_END(p)); }
TC_CASE(base16_0xg) { char *e = (char *)1; p = "0xg"; CHECK_EQ_I(strtol(p, &e, 16), 0); CHECK_EQ_P(e, EXP_END(p)); }
TC_CASE(base0_0x) { char *e = (char *)1; p = "0x"; CHECK_EQ_I(strtol(p, &e, 0), 0); CHECK_EQ_P(e, EXP_END(p)); }
TC_CASE(strtoul_0x) { char *e = (char *)1; p = "0x"; CHECK_EQ_U(strtoul(p, &e, 16), 0); CHECK_EQ_P(e, EXP_END(p)); }
TC_CASE(i64_0x) { char *e = (char *)1; p = "0x"; CHECK_EQ_I(_strtoi64(p, &e, 16), 0); CHECK_EQ_P(e, EXP_END(p)); }

int main(void) { tc_begin(); TC_RUN(base16_0x); TC_RUN(base16_0xg); TC_RUN(base0_0x); TC_RUN(strtoul_0x); TC_RUN(i64_0x); return tc_end(); }

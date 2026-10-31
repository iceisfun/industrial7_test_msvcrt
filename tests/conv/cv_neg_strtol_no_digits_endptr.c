/* TC
 * id:    cv_neg_strtol_no_digits_endptr
 * area:  conv
 * funcs: strtol strtoul _strtoi64

 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4p7 (no conversion: nptr stored in *endptr, returns 0)

 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile p;

#define NODIG(s, b, off) do { char *e = (char *)1; p = (s); \
    CHECK_EQ_I(strtol(p, &e, (b)), 0); CHECK_EQ_P(e, p + (off)); \
    e = (char *)1; CHECK_EQ_U(strtoul(p, &e, (b)), 0); CHECK_EQ_P(e, p + (off)); \
    e = (char *)1; CHECK_EQ_I(_strtoi64(p, &e, (b)), 0); CHECK_EQ_P(e, p + (off)); } while (0)

TC_CASE(letters) { NODIG("abc", 10, 0); }
TC_CASE(ws_sign) { NODIG("  -", 10, 0); }
TC_CASE(sign_only) { NODIG("+", 10, 0); NODIG("-", 10, 0); }
TC_CASE(empty) { NODIG("", 10, 0); }
TC_CASE(ws_only) { NODIG("   ", 10, 0); }
TC_CASE(sign_then_nondigit) { NODIG("-x", 10, 0); NODIG("+ 5", 10, 0); }

int main(void) { tc_begin(); TC_RUN(letters); TC_RUN(ws_sign); TC_RUN(sign_only); TC_RUN(empty); TC_RUN(ws_only); TC_RUN(sign_then_nondigit); return tc_end(); }

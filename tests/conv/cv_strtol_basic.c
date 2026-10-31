/* TC
 * id:    cv_strtol_basic
 * area:  conv
 * funcs: strtol

 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.4; MS docs strtol

 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile p;

TC_CASE(plain) { char *e; p = "123"; CHECK_EQ_I(strtol(p, &e, 10), 123); CHECK_EQ_P(e, p + 3); }
TC_CASE(negative) { char *e; p = "-45"; CHECK_EQ_I(strtol(p, &e, 10), -45); CHECK_EQ_P(e, p + 3); }
TC_CASE(plus) { char *e; p = "+7"; CHECK_EQ_I(strtol(p, &e, 10), 7); CHECK_EQ_P(e, p + 2); }
TC_CASE(ws_prefix) { char *e; p = "  \t 42"; CHECK_EQ_I(strtol(p, &e, 10), 42); CHECK_EQ_P(e, p + 6); }
TC_CASE(trailing) { char *e; p = "99abc"; CHECK_EQ_I(strtol(p, &e, 10), 99); CHECK_EQ_P(e, p + 2); p = "12 34"; CHECK_EQ_I(strtol(p, &e, 10), 12); CHECK_EQ_P(e, p + 2); }
TC_CASE(no_digits) { char *e = (char *)1; p = "xyz"; CHECK_EQ_I(strtol(p, &e, 10), 0); CHECK_EQ_P(e, p); }
TC_CASE(empty) { char *e = (char *)1; p = ""; CHECK_EQ_I(strtol(p, &e, 10), 0); CHECK_EQ_P(e, p); }
TC_CASE(endptr_null) { p = "77z"; CHECK_EQ_I(strtol(p, NULL, 10), 77); }

int main(void) { tc_begin(); TC_RUN(plain); TC_RUN(negative); TC_RUN(plus); TC_RUN(ws_prefix); TC_RUN(trailing); TC_RUN(no_digits); TC_RUN(empty); TC_RUN(endptr_null); return tc_end(); }

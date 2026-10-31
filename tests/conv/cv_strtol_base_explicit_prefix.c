/* TC
 * id:    cv_strtol_base_explicit_prefix
 * area:  conv
 * funcs: strtol

 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.4p3
 * note:  The base-2 '0b' prefix (C23) is probed separately in cv_strtol_base2_0b_probe.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile p;

TC_CASE(hex_with_prefix) { char *e; p = "0x1f"; CHECK_EQ_I(strtol(p, &e, 16), 31); CHECK_EQ_P(e, p + 4); }
TC_CASE(hex_without_prefix) { char *e; p = "1f"; CHECK_EQ_I(strtol(p, &e, 16), 31); CHECK_EQ_P(e, p + 2); }
TC_CASE(octal) { char *e; p = "0777"; CHECK_EQ_I(strtol(p, &e, 8), 511); CHECK_EQ_P(e, p + 4); p = "789"; CHECK_EQ_I(strtol(p, &e, 8), 7); CHECK_EQ_P(e, p + 1); }
TC_CASE(binary) { char *e; p = "1011"; CHECK_EQ_I(strtol(p, &e, 2), 11); CHECK_EQ_P(e, p + 4); p = "1012"; CHECK_EQ_I(strtol(p, &e, 2), 5); CHECK_EQ_P(e, p + 3); }
TC_CASE(base10_no_hex) { char *e; p = "0x10"; CHECK_EQ_I(strtol(p, &e, 10), 0); CHECK_EQ_P(e, p + 1); }

int main(void) { tc_begin(); TC_RUN(hex_with_prefix); TC_RUN(hex_without_prefix); TC_RUN(octal); TC_RUN(binary); TC_RUN(base10_no_hex); return tc_end(); }

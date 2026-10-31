/* TC
 * id:    cv_strtol_base_auto
 * area:  conv
 * funcs: strtol

 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.4p3 (base 0)

 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile p;

TC_CASE(hex) { char *e; p = "0x1F"; CHECK_EQ_I(strtol(p, &e, 0), 31); CHECK_EQ_P(e, p + 4); p = "0X1f"; CHECK_EQ_I(strtol(p, &e, 0), 31); CHECK_EQ_P(e, p + 4); }
TC_CASE(octal) { char *e; p = "010"; CHECK_EQ_I(strtol(p, &e, 0), 8); CHECK_EQ_P(e, p + 3); }
TC_CASE(zero) { char *e; p = "0"; CHECK_EQ_I(strtol(p, &e, 0), 0); CHECK_EQ_P(e, p + 1); }
TC_CASE(bad_octal_digit) { char *e; p = "09"; CHECK_EQ_I(strtol(p, &e, 0), 0); CHECK_EQ_P(e, p + 1); }
TC_CASE(signed_prefix) { char *e; p = "-0x10"; CHECK_EQ_I(strtol(p, &e, 0), -16); CHECK_EQ_P(e, p + 5); p = "+010"; CHECK_EQ_I(strtol(p, &e, 0), 8); CHECK_EQ_P(e, p + 4); }
TC_CASE(decimal) { char *e; p = "19"; CHECK_EQ_I(strtol(p, &e, 0), 19); CHECK_EQ_P(e, p + 2); }

int main(void) { tc_begin(); TC_RUN(hex); TC_RUN(octal); TC_RUN(zero); TC_RUN(bad_octal_digit); TC_RUN(signed_prefix); TC_RUN(decimal); return tc_end(); }

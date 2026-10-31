/* TC
 * id:    cv_neg_strtol_overflow
 * area:  conv
 * funcs: strtol

 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4p8 (LONG_MAX/LONG_MIN and ERANGE); MS docs strtol

 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static const char *volatile p;

TC_CASE(pos) { char *e; errno = 0; p = "2147483648"; CHECK_EQ_I(strtol(p, &e, 10), LONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, p + 10); }
TC_CASE(neg) { char *e; errno = 0; p = "-2147483649"; CHECK_EQ_I(strtol(p, &e, 10), LONG_MIN); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, p + 11); }
TC_CASE(huge) { char *e; errno = 0; p = "99999999999999999999999999999999999999x"; CHECK_EQ_I(strtol(p, &e, 10), LONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, p + 38); }
TC_CASE(huge_neg) { char *e; errno = 0; p = "-99999999999999999999999999999999999999"; CHECK_EQ_I(strtol(p, &e, 10), LONG_MIN); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, p + 39); }
TC_CASE(hex_ffffffff) { char *e; errno = 0; p = "0xffffffff"; CHECK_EQ_I(strtol(p, &e, 16), LONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, p + 10); }
TC_CASE(base36) { char *e; errno = 0; p = "zzzzzzzz"; CHECK_EQ_I(strtol(p, &e, 36), LONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, p + 8); }
TC_CASE(just_below_ok) { char *e; errno = 0; p = "2147483647"; CHECK_EQ_I(strtol(p, &e, 10), LONG_MAX); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(pos); TC_RUN(neg); TC_RUN(huge); TC_RUN(huge_neg); TC_RUN(hex_ffffffff); TC_RUN(base36); TC_RUN(just_below_ok); return tc_end(); }

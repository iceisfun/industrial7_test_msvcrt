/* TC
 * id:    cv_strtol_limits
 * area:  conv
 * funcs: strtol

 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.4, 5.2.4.2.1 (LONG_MAX is 2147483647 on Windows LLP64)

 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static const char *volatile p;

TC_CASE(max) { char *e; errno = EDOM; p = "2147483647"; CHECK_EQ_I(strtol(p, &e, 10), 2147483647L); CHECK_EQ_I(strtol(p, &e, 10), LONG_MAX); CHECK_EQ_P(e, p + 10); CHECK_EQ_I(errno, EDOM); }
TC_CASE(min) { char *e; errno = EDOM; p = "-2147483648"; CHECK_EQ_I(strtol(p, &e, 10), LONG_MIN); CHECK_EQ_P(e, p + 11); CHECK_EQ_I(errno, EDOM); }
TC_CASE(hex_max) { char *e; errno = EDOM; p = "0x7fffffff"; CHECK_EQ_I(strtol(p, &e, 16), LONG_MAX); CHECK_EQ_P(e, p + 10); CHECK_EQ_I(errno, EDOM); }
TC_CASE(hex_min) { char *e; errno = EDOM; p = "-0x80000000"; CHECK_EQ_I(strtol(p, &e, 16), LONG_MIN); CHECK_EQ_P(e, p + 11); CHECK_EQ_I(errno, EDOM); }
TC_CASE(long_zeros) { char *e; errno = EDOM; p = "000000000000000000012"; CHECK_EQ_I(strtol(p, &e, 10), 12); CHECK_EQ_P(e, p + 21); CHECK_EQ_I(errno, EDOM); }

int main(void) { tc_begin(); TC_RUN(max); TC_RUN(min); TC_RUN(hex_max); TC_RUN(hex_min); TC_RUN(long_zeros); return tc_end(); }

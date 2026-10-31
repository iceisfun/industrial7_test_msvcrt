/* TC
 * id:    ucr_strtoul_negative_wrap
 * area:  ucrt_only
 * funcs: strtoul
 * crt:   all
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.22.1.4 (value negated in the return type)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static const char *volatile v;
TC_CASE(minus_one) { char *e; v = "-1"; errno = 0; CHECK_EQ_U(strtoul((const char *)v, &e, 10), ULONG_MAX); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, v + 2); }
TC_CASE(minus_zero) { char *e; v = "-0"; errno = 0; CHECK_EQ_U(strtoul((const char *)v, &e, 10), 0); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, v + 2); }
TC_CASE(minus_max) { char *e; v = "-4294967295"; errno = 0; CHECK_EQ_U(strtoul((const char *)v, &e, 10), 1); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, v + 11); }

int main(void) { tc_begin(); TC_RUN(minus_one); TC_RUN(minus_zero); TC_RUN(minus_max); return tc_end(); }

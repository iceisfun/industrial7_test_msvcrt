/* TC
 * id:    cv_strtoul_negation
 * area:  conv
 * funcs: strtoul _strtoui64
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.4p5 (negation in unsigned type); MS docs strtoul
 * note:  No ERANGE for in-range magnitudes
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static unsigned long U(const char *volatile s) { char *e; errno = 0; return strtoul(s, &e, 10); }

TC_CASE(minus_one) { CHECK_EQ_U(U("-1"), ULONG_MAX); CHECK_EQ_I(errno, 0); }
TC_CASE(minus_two) { CHECK_EQ_U(U("-2"), ULONG_MAX - 1); CHECK_EQ_I(errno, 0); }
TC_CASE(minus_zero) { CHECK_EQ_U(U("-0"), 0); CHECK_EQ_I(errno, 0); }
TC_CASE(minus_max) { CHECK_EQ_U(U("-4294967295"), 1); CHECK_EQ_I(errno, 0); }
TC_CASE(u64_minus_one)
{
    volatile const char *s = "-1"; char *e; unsigned __int64 r;
    errno = 0; r = _strtoui64((const char *)s, &e, 10);
    CHECK_EQ_U(r, ULLONG_MAX); CHECK_EQ_I(errno, 0);
}

int main(void) { tc_begin(); TC_RUN(minus_one); TC_RUN(minus_two); TC_RUN(minus_zero); TC_RUN(minus_max); TC_RUN(u64_minus_one); return tc_end(); }

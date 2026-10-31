/* TC
 * id:    cv_neg_strtoll_overflow
 * area:  conv
 * funcs: strtoll strtoull
 * crt:   >=120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4p8; MS docs strtoll
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

TC_CASE(pos)
{
    volatile const char *s = "9223372036854775808"; char *e; long long r;
    errno = 0; r = strtoll((const char *)s, &e, 10);
    CHECK_EQ_I(r, LLONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 19);
}
TC_CASE(neg)
{
    volatile const char *s = "-9223372036854775809"; char *e; long long r;
    errno = 0; r = strtoll((const char *)s, &e, 10);
    CHECK_EQ_I(r, LLONG_MIN); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 20);
}
TC_CASE(ull_2p64)
{
    volatile const char *s = "18446744073709551616"; char *e; unsigned long long r;
    errno = 0; r = strtoull((const char *)s, &e, 10);
    CHECK_EQ_U(r, ULLONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 20);
}
TC_CASE(endptr_past_digits)
{
    volatile const char *s = "99999999999999999999xyz"; char *e = 0; long long r;
    errno = 0; r = strtoll((const char *)s, &e, 10);
    CHECK_EQ_I(r, LLONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 20);
}

int main(void) { tc_begin(); TC_RUN(pos); TC_RUN(neg); TC_RUN(ull_2p64); TC_RUN(endptr_past_digits); return tc_end(); }

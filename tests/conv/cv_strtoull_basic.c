/* TC
 * id:    cv_strtoull_basic
 * area:  conv
 * funcs: strtoull
 * crt:   >=120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.4; MS docs strtoull
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(max)
{
    volatile const char *s = "18446744073709551615"; char *e;
    CHECK_EQ_U(strtoull((const char *)s, &e, 10), 0xffffffffffffffffULL); CHECK_EQ_P(e, s + 20);
}
TC_CASE(hex_max)
{
    volatile const char *s = "0xFFFFFFFFFFFFFFFF"; char *e;
    CHECK_EQ_U(strtoull((const char *)s, &e, 16), 0xffffffffffffffffULL); CHECK_EQ_P(e, s + 18);
    errno = 0; CHECK_EQ_U(strtoull((const char *)s, &e, 0), 0xffffffffffffffffULL); CHECK_EQ_I(errno, 0);
}
TC_CASE(minus_one_wraps)
{
    volatile const char *s = "-1"; char *e; errno = 0;
    CHECK_EQ_U(strtoull((const char *)s, &e, 10), 0xffffffffffffffffULL); CHECK_EQ_I(errno, 0);
}
TC_CASE(octal)
{
    volatile const char *s = "0777"; char *e;
    CHECK_EQ_U(strtoull((const char *)s, &e, 0), 511); CHECK_EQ_P(e, s + 4);
    s = "0789"; CHECK_EQ_U(strtoull((const char *)s, &e, 0), 7); CHECK_EQ_P(e, s + 2);
}
TC_CASE(endptr_nodigits)
{
    volatile const char *s = "  g"; char *e = 0;
    CHECK_EQ_U(strtoull((const char *)s, &e, 10), 0); CHECK_EQ_P(e, s);
}

int main(void) { tc_begin(); TC_RUN(max); TC_RUN(hex_max); TC_RUN(minus_one_wraps); TC_RUN(octal); TC_RUN(endptr_nodigits); return tc_end(); }

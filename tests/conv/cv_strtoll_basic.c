/* TC
 * id:    cv_strtoll_basic
 * area:  conv
 * funcs: strtoll
 * crt:   >=120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.4; MS docs strtoll
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static long long L(const char *volatile s, int base, char **e) { return strtoll(s, e, base); }

TC_CASE(limits)
{
    char *e;
    CHECK_EQ_I(L("9223372036854775807", 10, &e), 0x7fffffffffffffffLL);
    CHECK_EQ_I(L("-9223372036854775808", 10, &e), -0x7fffffffffffffffLL - 1);
}
TC_CASE(bases)
{
    char *e;
    CHECK_EQ_I(L("ff", 16, &e), 255); CHECK_EQ_I(L("0xFf", 16, &e), 255);
    CHECK_EQ_I(L("0x1f", 0, &e), 31); CHECK_EQ_I(L("017", 0, &e), 15); CHECK_EQ_I(L("17", 0, &e), 17);
    CHECK_EQ_I(L("zz", 36, &e), 35 * 36 + 35); CHECK_EQ_I(L("-Z", 36, &e), -35);
    CHECK_EQ_I(L("101", 2, &e), 5);
}
TC_CASE(endptr)
{
    volatile const char *s = "123abc"; char *e = 0;
    CHECK_EQ_I(strtoll((const char *)s, &e, 10), 123); CHECK_EQ_P(e, s + 3);
    s = "xyz"; CHECK_EQ_I(strtoll((const char *)s, &e, 10), 0); CHECK_EQ_P(e, s);
}
TC_CASE(whitespace)
{
    volatile const char *s = " \t\n\v\f\r -42x"; char *e = 0;
    CHECK_EQ_I(strtoll((const char *)s, &e, 10), -42); CHECK_EQ_P(e, s + 10);
    s = "+7"; CHECK_EQ_I(strtoll((const char *)s, &e, 10), 7);
}

int main(void) { tc_begin(); TC_RUN(limits); TC_RUN(bases); TC_RUN(endptr); TC_RUN(whitespace); return tc_end(); }

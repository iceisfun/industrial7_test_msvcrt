/* TC
 * id:    cv_strtoi64_basic
 * area:  conv
 * funcs: _strtoi64 _strtoui64
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strtoi64, _strtoui64
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static __int64 S(const char *volatile s, int b, char **e) { return _strtoi64(s, e, b); }
static unsigned __int64 U(const char *volatile s, int b, char **e) { return _strtoui64(s, e, b); }

TC_CASE(limits)
{
    char *e;
    CHECK_EQ_I(S("9223372036854775807", 10, &e), 0x7fffffffffffffffLL);
    CHECK_EQ_I(S("-9223372036854775808", 10, &e), -0x7fffffffffffffffLL - 1);
    CHECK_EQ_U(U("18446744073709551615", 10, &e), 0xffffffffffffffffULL);
}
TC_CASE(bases)
{
    char *e;
    CHECK_EQ_I(S("ff", 16, &e), 255); CHECK_EQ_I(S("777", 8, &e), 511); CHECK_EQ_I(S("1010", 2, &e), 10);
    CHECK_EQ_I(S("zz", 36, &e), 1295); CHECK_EQ_U(U("Zz", 36, &e), 1295);
    CHECK_EQ_U(U("ffffffffffffffff", 16, &e), 0xffffffffffffffffULL);
}
TC_CASE(base0)
{
    char *e;
    CHECK_EQ_I(S("0x10", 0, &e), 16); CHECK_EQ_I(S("010", 0, &e), 8); CHECK_EQ_I(S("10", 0, &e), 10);
    CHECK_EQ_U(U("0X20", 0, &e), 32);
}
TC_CASE(endptr_ws)
{
    volatile const char *s = "  \t-12ab"; char *e = 0;
    CHECK_EQ_I(_strtoi64((const char *)s, &e, 10), -12); CHECK_EQ_P(e, s + 6);
    s = "  +9 "; CHECK_EQ_U(_strtoui64((const char *)s, &e, 10), 9); CHECK_EQ_P(e, s + 4);
}
TC_CASE(no_digits)
{
    volatile const char *s = "  zz"; char *e = 0;
    CHECK_EQ_I(_strtoi64((const char *)s, &e, 10), 0); CHECK_EQ_P(e, s);
    e = 0; CHECK_EQ_U(_strtoui64((const char *)s, &e, 10), 0); CHECK_EQ_P(e, s);
}

int main(void) { tc_begin(); TC_RUN(limits); TC_RUN(bases); TC_RUN(base0); TC_RUN(endptr_ws); TC_RUN(no_digits); return tc_end(); }

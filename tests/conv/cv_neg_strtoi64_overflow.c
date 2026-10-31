/* TC
 * id:    cv_neg_strtoi64_overflow
 * area:  conv
 * funcs: _strtoi64 _strtoui64
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtoi64: returns _I64_MAX/_I64_MIN/_UI64_MAX and sets errno ERANGE on overflow
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

TC_CASE(pos_2p63)
{
    volatile const char *s = "9223372036854775808"; char *e; __int64 r;
    errno = 0; r = _strtoi64((const char *)s, &e, 10);
    CHECK_EQ_I(r, _I64_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 19);
}
TC_CASE(neg_below_min)
{
    volatile const char *s = "-9223372036854775809"; char *e; __int64 r;
    errno = 0; r = _strtoi64((const char *)s, &e, 10);
    CHECK_EQ_I(r, _I64_MIN); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 20);
}
TC_CASE(u64_2p64)
{
    volatile const char *s = "18446744073709551616"; char *e; unsigned __int64 r;
    errno = 0; r = _strtoui64((const char *)s, &e, 10);
    CHECK_EQ_U(r, _UI64_MAX); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(hex_17_fs)
{
    volatile const char *s = "fffffffffffffffff"; char *e; unsigned __int64 u; __int64 r;
    errno = 0; u = _strtoui64((const char *)s, &e, 16);
    CHECK_EQ_U(u, _UI64_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 17);
    errno = 0; r = _strtoi64((const char *)s, &e, 16);
    CHECK_EQ_I(r, _I64_MAX); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(in_range_no_errno)
{
    volatile const char *s = "-9223372036854775808"; char *e; __int64 r;
    errno = 0; r = _strtoi64((const char *)s, &e, 10);
    CHECK_EQ_I(r, _I64_MIN); CHECK_EQ_I(errno, 0);
}

int main(void) { tc_begin(); TC_RUN(pos_2p63); TC_RUN(neg_below_min); TC_RUN(u64_2p64); TC_RUN(hex_17_fs); TC_RUN(in_range_no_errno); return tc_end(); }

/* TC
 * id:    cv_neg_strtoul_overflow
 * area:  conv
 * funcs: strtoul _strtoui64
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4p8 (ULONG_MAX, ERANGE); MS docs strtoul
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

TC_CASE(dec_2p32)
{
    volatile const char *s = "4294967296"; char *e; unsigned long r;
    errno = 0; r = strtoul((const char *)s, &e, 10);
    CHECK_EQ_U(r, ULONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 10);
}
TC_CASE(hex_overflow)
{
    volatile const char *s = "0x1ffffffff"; char *e; unsigned long r;
    errno = 0; r = strtoul((const char *)s, &e, 0);
    CHECK_EQ_U(r, ULONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 11);
}
TC_CASE(u64_2p64)
{
    volatile const char *s = "18446744073709551616"; char *e; unsigned __int64 r;
    errno = 0; r = _strtoui64((const char *)s, &e, 10);
    CHECK_EQ_U(r, ULLONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 20);
}
TC_CASE(max_ok)
{
    volatile const char *s = "4294967295"; char *e; unsigned long r;
    errno = 0; r = strtoul((const char *)s, &e, 10);
    CHECK_EQ_U(r, ULONG_MAX); CHECK_EQ_I(errno, 0);
}

int main(void) { tc_begin(); TC_RUN(dec_2p32); TC_RUN(hex_overflow); TC_RUN(u64_2p64); TC_RUN(max_ok); return tc_end(); }

/* TC
 * id:    cv_neg_strtoul_negative_range
 * area:  conv
 * funcs: strtoul strtoull _strtoui64 wcstoul
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4 (negation in the unsigned type; ERANGE + ULONG_MAX when the magnitude does not fit); MS docs strtoul
 * note:  On Windows unsigned long is 32 bits.
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static const char *volatile vs;
static const wchar_t *volatile vw;

TC_CASE(minus_one) { char *e; errno = 0; vs = "-1"; CHECK_EQ_U(strtoul(vs, &e, 10), ULONG_MAX); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, vs + 2); }
TC_CASE(wraps_to_one) { char *e; errno = 0; vs = "-4294967295"; CHECK_EQ_U(strtoul(vs, &e, 10), 1); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, vs + 11); }
TC_CASE(too_big_32) { char *e; errno = 0; vs = "-4294967296"; CHECK_EQ_U(strtoul(vs, &e, 10), ULONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, vs + 11); }
TC_CASE(ull_wraps) { char *e; errno = 0; vs = "-18446744073709551615"; CHECK_EQ_U(strtoull(vs, &e, 10), 1); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, vs + 21); }
TC_CASE(ull_too_big) { char *e; errno = 0; vs = "-18446744073709551616"; CHECK_EQ_U(strtoull(vs, &e, 10), ULLONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, vs + 21); }
TC_CASE(ui64_variants)
{
    char *e; errno = 0; vs = "-1"; CHECK_EQ_U(_strtoui64(vs, &e, 10), ULLONG_MAX); CHECK_EQ_I(errno, 0);
    errno = 0; vs = "-18446744073709551616"; CHECK_EQ_U(_strtoui64(vs, &e, 10), ULLONG_MAX); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(wide)
{
    wchar_t *e; errno = 0; vw = L"-1"; CHECK_EQ_U(wcstoul(vw, &e, 10), ULONG_MAX); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, vw + 2);
    errno = 0; vw = L"-4294967296"; CHECK_EQ_U(wcstoul(vw, &e, 10), ULONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, vw + 11);
}
int main(void)
{
    tc_begin();
    TC_RUN(minus_one); TC_RUN(wraps_to_one); TC_RUN(too_big_32); TC_RUN(ull_wraps); TC_RUN(ull_too_big); TC_RUN(ui64_variants); TC_RUN(wide);
    return tc_end();
}

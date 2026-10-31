/* TC
 * id:    gst_strtoi64_l_limits_neg_overflow
 * area:  conv
 * funcs: _strtoi64_l _strtoui64_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtoll/strtoull (LLONG_MAX/LLONG_MIN/ULLONG_MAX and errno ERANGE on overflow); C11 7.22.1.4
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <errno.h>

#define I64MIN (-9223372036854775807LL - 1)
static const char *volatile v;
static _locale_t loc;

TC_CASE(signed_overflow)
{
    char *e; long long r;
    v = "9223372036854775808"; errno = 0; r = _strtoi64_l(v, &e, 10, loc);
    CHECK_EQ_I(r, 9223372036854775807LL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 19);
    v = "-9223372036854775809"; errno = 0; r = _strtoi64_l(v, &e, 10, loc);
    CHECK_EQ_I(r, I64MIN); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 20);
}
TC_CASE(unsigned_overflow)
{
    char *e; unsigned long long r;
    v = "18446744073709551616"; errno = 0; r = _strtoui64_l(v, &e, 10, loc);
    CHECK_EQ_U(r, 0xFFFFFFFFFFFFFFFFULL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 20);
}
TC_CASE(long_digit_string)
{
    char *e; long long r; unsigned long long u;
    v = "99999999999999999999999999999999999999xyz"; errno = 0; r = _strtoi64_l(v, &e, 10, loc);
    CHECK_EQ_I(r, 9223372036854775807LL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 38);
    errno = 0; u = _strtoui64_l(v, &e, 10, loc);
    CHECK_EQ_U(u, 0xFFFFFFFFFFFFFFFFULL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 38);
    v = "-99999999999999999999999999"; errno = 0; r = _strtoi64_l(v, &e, 10, loc);
    CHECK_EQ_I(r, I64MIN); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 27);
}
TC_CASE(hex_overflow)
{
    char *e; long long r; unsigned long long u;
    v = "0x1FFFFFFFFFFFFFFFF"; errno = 0; u = _strtoui64_l(v, &e, 16, loc);
    CHECK_EQ_U(u, 0xFFFFFFFFFFFFFFFFULL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 19);
    errno = 0; r = _strtoi64_l(v, &e, 16, loc);
    CHECK_EQ_I(r, 9223372036854775807LL); CHECK_EQ_I(errno, ERANGE);
    v = "0x8000000000000000"; errno = 0; r = _strtoi64_l(v, &e, 16, loc);
    CHECK_EQ_I(r, 9223372036854775807LL); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(errno_not_cleared_on_success)
{
    char *e; long long r; unsigned long long u;
    v = "12345"; errno = 0x55; r = _strtoi64_l(v, &e, 10, loc);
    CHECK_EQ_I(r, 12345); CHECK_EQ_I(errno, 0x55);
    errno = 0x56; u = _strtoui64_l(v, &e, 10, loc);
    CHECK_EQ_U(u, 12345); CHECK_EQ_I(errno, 0x56);
    errno = 0;
}
TC_CASE(boundaries_no_erange)
{
    char *e;
    v = "9223372036854775807"; errno = 0; CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), 9223372036854775807LL); CHECK_EQ_I(errno, 0);
    v = "-9223372036854775808"; errno = 0; CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), I64MIN); CHECK_EQ_I(errno, 0);
    v = "18446744073709551615"; errno = 0; CHECK_EQ_U(_strtoui64_l(v, &e, 10, loc), 0xFFFFFFFFFFFFFFFFULL); CHECK_EQ_I(errno, 0);
    v = "0xFFFFFFFFFFFFFFFF"; errno = 0; CHECK_EQ_U(_strtoui64_l(v, &e, 16, loc), 0xFFFFFFFFFFFFFFFFULL); CHECK_EQ_I(errno, 0);
}
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    CHECK(loc != NULL);
    TC_RUN(signed_overflow); TC_RUN(unsigned_overflow); TC_RUN(long_digit_string); TC_RUN(hex_overflow);
    TC_RUN(errno_not_cleared_on_success); TC_RUN(boundaries_no_erange);
    _free_locale(loc);
    return tc_end();
}

/* TC
 * id:    cv_wcstoll_basic
 * area:  conv
 * funcs: wcstoll wcstoull
 * crt:   >=120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.4.1.2; MS docs strtoll, wcstoll (VS2013+)
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static const wchar_t *volatile vs;

TC_CASE(int64_max) { vs = L"9223372036854775807"; CHECK_EQ_I(wcstoll(vs, NULL, 10), 9223372036854775807LL); vs = L"-9223372036854775808"; CHECK(wcstoll(vs, NULL, 10) == LLONG_MIN); }
TC_CASE(uint64_max) { vs = L"18446744073709551615"; CHECK(wcstoull(vs, NULL, 10) == ULLONG_MAX); }
TC_CASE(neg_wrap) { vs = L"-1"; CHECK(wcstoull(vs, NULL, 10) == ULLONG_MAX); }
TC_CASE(base16) { vs = L"0xFFFFFFFFFF"; CHECK_EQ_U(wcstoull(vs, NULL, 16), 0xFFFFFFFFFFULL); CHECK_EQ_I(wcstoll(vs, NULL, 0), 0xFFFFFFFFFFLL); }
TC_CASE(overflow)
{
    errno = 0; vs = L"9223372036854775808"; CHECK(wcstoll(vs, NULL, 10) == LLONG_MAX); CHECK_EQ_I(errno, ERANGE);
    errno = 0; vs = L"-9223372036854775809"; CHECK(wcstoll(vs, NULL, 10) == LLONG_MIN); CHECK_EQ_I(errno, ERANGE);
    errno = 0; vs = L"18446744073709551616"; CHECK(wcstoull(vs, NULL, 10) == ULLONG_MAX); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(endptr)
{
    wchar_t *e = NULL; vs = L" 77zz"; CHECK_EQ_I(wcstoll(vs, &e, 10), 77); CHECK_EQ_P(e, vs + 3);
    vs = L"zz"; CHECK_EQ_I(wcstoull(vs, &e, 10), 0); CHECK_EQ_P(e, vs);
}

int main(void) { tc_begin(); TC_RUN(int64_max); TC_RUN(uint64_max); TC_RUN(neg_wrap); TC_RUN(base16); TC_RUN(overflow); TC_RUN(endptr); return tc_end(); }

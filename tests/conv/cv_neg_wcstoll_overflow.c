/* TC
 * id:    cv_neg_wcstoll_overflow
 * area:  conv
 * funcs: wcstoll _wcstoi64
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4p8 (LLONG_MAX/LLONG_MIN + ERANGE); MS docs strtoll, wcstoll, _wcstoi64
 */
#include "tc.h"
#include <wchar.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static const wchar_t *volatile v_s;

TC_CASE(pos_overflow)
{
    wchar_t *e = (wchar_t *)1; long long r; static const wchar_t s[] = L"9223372036854775808";
    v_s = s; errno = 0; r = wcstoll(v_s, &e, 10);
    CHECK_EQ_I(r, LLONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 19);
    v_s = s; errno = 0; r = _wcstoi64(v_s, &e, 10);
    CHECK_EQ_I(r, LLONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 19);
}
TC_CASE(neg_overflow)
{
    wchar_t *e = (wchar_t *)1; long long r; static const wchar_t s[] = L"-9223372036854775809";
    v_s = s; errno = 0; r = wcstoll(v_s, &e, 10);
    CHECK_EQ_I(r, LLONG_MIN); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 20);
    v_s = s; errno = 0; r = _wcstoi64(v_s, &e, 10);
    CHECK_EQ_I(r, LLONG_MIN); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(exact_limits)
{
    wchar_t *e = (wchar_t *)1; long long r;
    static const wchar_t a[] = L"9223372036854775807", b[] = L"-9223372036854775808";
    v_s = a; errno = 0; r = wcstoll(v_s, &e, 10);
    CHECK_EQ_I(r, LLONG_MAX); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, a + 19);
    v_s = b; errno = 0; r = wcstoll(v_s, &e, 10);
    CHECK_EQ_I(r, LLONG_MIN); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, b + 20);
}
TC_CASE(hex_overflow)
{
    wchar_t *e = (wchar_t *)1; long long r; static const wchar_t s[] = L"8000000000000000";
    v_s = s; errno = 0; r = wcstoll(v_s, &e, 16);
    CHECK_EQ_I(r, LLONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 16);
}
TC_CASE(overflow_trailing_text)
{
    wchar_t *e = (wchar_t *)1; long long r; static const wchar_t s[] = L"9223372036854775808abc", n[] = L"-9223372036854775809xyz";
    v_s = s; errno = 0; r = wcstoll(v_s, &e, 10);
    CHECK_EQ_I(r, LLONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 19); CHECK(*e == L'a');
    v_s = n; errno = 0; r = wcstoll(v_s, &e, 10);
    CHECK_EQ_I(r, LLONG_MIN); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, n + 20); CHECK(*e == L'x');
}

int main(void)
{
    tc_begin();
    TC_RUN(pos_overflow); TC_RUN(neg_overflow); TC_RUN(exact_limits); TC_RUN(hex_overflow); TC_RUN(overflow_trailing_text);
    return tc_end();
}

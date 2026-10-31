/* TC
 * id:    cv_wcstoi64_basic
 * area:  conv
 * funcs: _wcstoi64 _wcstoui64
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strtoll, _strtoll_l, wcstoll, _wcstoll_l (_wcstoi64 / _wcstoui64)
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>

static const wchar_t *volatile vs;

TC_CASE(limits)
{
    vs = L"9223372036854775807"; CHECK_EQ_I(_wcstoi64(vs, NULL, 10), 0x7FFFFFFFFFFFFFFFLL);
    vs = L"-9223372036854775808"; CHECK_EQ_U((unsigned long long)_wcstoi64(vs, NULL, 10), 0x8000000000000000ULL);
    vs = L"18446744073709551615"; CHECK_EQ_U(_wcstoui64(vs, NULL, 10), 0xFFFFFFFFFFFFFFFFULL);
}
TC_CASE(bases)
{
    vs = L"1010"; CHECK_EQ_I(_wcstoi64(vs, NULL, 2), 10);
    vs = L"0xdeadbeefcafe"; CHECK_EQ_U(_wcstoui64(vs, NULL, 16), 0xdeadbeefcafeULL);
    vs = L"zz"; CHECK_EQ_I(_wcstoi64(vs, NULL, 36), 1295);
    vs = L"-ff"; CHECK_EQ_I(_wcstoi64(vs, NULL, 16), -255);
}
TC_CASE(whitespace) { wchar_t *e; vs = L" \t\n 12345678901 x"; CHECK_EQ_I(_wcstoi64(vs, &e, 10), 12345678901LL); CHECK_EQ_P(e, vs + 15); }
TC_CASE(no_digits)
{
    wchar_t *e = NULL; vs = L"  q"; CHECK_EQ_I(_wcstoi64(vs, &e, 10), 0); CHECK_EQ_P(e, vs);
    e = NULL; CHECK_EQ_U(_wcstoui64(vs, &e, 10), 0); CHECK_EQ_P(e, vs);
}

int main(void) { tc_begin(); TC_RUN(limits); TC_RUN(bases); TC_RUN(whitespace); TC_RUN(no_digits); return tc_end(); }

/* TC
 * id:    gcv_wtoll_basic
 * area:  conv
 * funcs: _wtoll _wtoll_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _wtoi, _wtoi_l, _wtoll
 * note:  _wtoll* absent from msvcr110 and older.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static _locale_t L;
static long long W(const wchar_t *volatile s) { return _wtoll(s); }
static long long WL(const wchar_t *volatile s) { return _wtoll_l(s, L); }

TC_CASE(simple) { CHECK_EQ_I(W(L"123"), 123LL); CHECK_EQ_I(W(L" \t -45"), -45LL); CHECK_EQ_I(W(L"+8"), 8LL); }
TC_CASE(limits) { CHECK_EQ_I(W(L"9223372036854775807"), 0x7fffffffffffffffLL); CHECK_EQ_I(W(L"-9223372036854775808"), -0x7fffffffffffffffLL - 1); }
TC_CASE(trailing) { CHECK_EQ_I(W(L"5000000000xyz"), 5000000000LL); CHECK_EQ_I(W(L"3.9"), 3LL); }
TC_CASE(locale_same)
{
    CHECK_EQ_I(WL(L"  -987654321012"), -987654321012LL);
    CHECK_EQ_I(WL(L"9223372036854775807"), 0x7fffffffffffffffLL);
    CHECK_EQ_I(WL(L"77z"), W(L"77z"));
}
TC_CASE(no_digits) { CHECK_EQ_I(W(L""), 0LL); CHECK_EQ_I(W(L"x1"), 0LL); CHECK_EQ_I(WL(L""), 0LL); CHECK_EQ_I(WL(L"-"), 0LL); }

int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    CHECK(L != NULL);
    if (!L) return tc_end() | 1;
    TC_RUN(simple); TC_RUN(limits); TC_RUN(trailing); TC_RUN(locale_same); TC_RUN(no_digits);
    _free_locale(L);
    return tc_end();
}

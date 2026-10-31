/* TC
 * id:    gcv_wtoi_l_family_basic
 * area:  conv
 * funcs: _wtoi_l _wtol_l _wtoi64_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _wtoi, _wtoi_l, _wtol, _wtol_l, _wtoi64, _wtoi64_l
 * note:  msvcrt.dll dropped: no _create_locale import. Whitespace set is the C-locale isspace set.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static void *L;
static int  I(const wchar_t *volatile s) { return _wtoi_l(s, (_locale_t)L); }
static long Lg(const wchar_t *volatile s) { return _wtol_l(s, (_locale_t)L); }
static __int64 Q(const wchar_t *volatile s) { return _wtoi64_l(s, (_locale_t)L); }

TC_CASE(wtoi) { CHECK_EQ_I(I(L" -42xyz"), -42); CHECK_EQ_I(I(L"+7"), 7); CHECK_EQ_I(I(L"2147483647"), 2147483647); }
TC_CASE(wtol) { CHECK_EQ_I(Lg(L"2147483647"), 2147483647L); CHECK_EQ_I(Lg(L"-2147483648"), -2147483647L - 1); CHECK_EQ_I(Lg(L"  15z"), 15L); }
TC_CASE(wtoi64) { CHECK_EQ_I(Q(L"9223372036854775807"), 0x7fffffffffffffffLL); CHECK_EQ_I(Q(L"-9223372036854775808"), -0x7fffffffffffffffLL - 1); CHECK_EQ_I(Q(L"-1"), -1LL); }
TC_CASE(whitespace)
{
    CHECK_EQ_I(I(L"\t\n\v\f\r 5"), 5);
    CHECK_EQ_I(Lg(L"\n\n9"), 9L);
    CHECK_EQ_I(Q(L"\v\f-3"), -3LL);
}
TC_CASE(wide_stops) { CHECK_EQ_I(I(L"12\x0100" L"34"), 12); CHECK_EQ_I(Q(L"98\x0100"), 98LL); CHECK_EQ_I(Lg(L"\x0100" L"5"), 0L); }

int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    CHECK(L != NULL);
    if (!L) return tc_end() | 1;
    TC_RUN(wtoi); TC_RUN(wtol); TC_RUN(wtoi64); TC_RUN(whitespace); TC_RUN(wide_stops);
    _free_locale((_locale_t)L);
    return tc_end();
}

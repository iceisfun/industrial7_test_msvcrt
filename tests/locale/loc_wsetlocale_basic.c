/* TC
 * id:    loc_wsetlocale_basic
 * area:  locale
 * funcs: _wsetlocale
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _wsetlocale, setlocale
 */
#include "tc.h"
#include <locale.h>
#include <wchar.h>

TC_CASE(query_C) { CHECK_WSTR(_wsetlocale(LC_ALL, NULL), L"C"); }
TC_CASE(set_english)
{
    volatile const wchar_t *n = L"English_United States.1252";
    wchar_t *r = _wsetlocale(LC_ALL, (const wchar_t *)n);
    CHECK(r != NULL);
    CHECK_WSTR(r, L"English_United States.1252");
    _wsetlocale(LC_ALL, L"C");
}
TC_CASE(roundtrip_wide)
{
    volatile const wchar_t *n = L"English_United States.1252";
    wchar_t *r = _wsetlocale(LC_ALL, (const wchar_t *)n);
    wchar_t *q;
    CHECK(r != NULL);
    q = _wsetlocale(LC_ALL, NULL);
    CHECK_WSTR(q, L"English_United States.1252");
    _wsetlocale(LC_ALL, L"C");
    CHECK_WSTR(_wsetlocale(LC_ALL, NULL), L"C");
}
TC_CASE(narrow_wide_agree_via_setlocale)
{
    volatile const wchar_t *n = L"English_United States.1252";
    char *c;
    if (!_wsetlocale(LC_ALL, (const wchar_t *)n)) TC_SKIP("locale unavailable");
    c = setlocale(LC_ALL, NULL);
    CHECK_STR(c, "English_United States.1252");
    _wsetlocale(LC_ALL, L"C");
    CHECK_STR(setlocale(LC_ALL, NULL), "C");
}
int main(void) { tc_begin(); TC_RUN(query_C); TC_RUN(set_english); TC_RUN(roundtrip_wide); TC_RUN(narrow_wide_agree_via_setlocale); return tc_end(); }

/* TC
 * id:    loc_probe_lc_locale_name_func
 * area:  locale
 * funcs: ___lc_locale_name_func setlocale
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p2
 * conf:  probe
 * ref:   none (internal helper)
 * note:  PROBE: ___lc_locale_name_func() returns a wchar_t*[6] indexed by LC_* category. After
 *        setlocale(LC_ALL,"English_United States.1252") the LC_CTYPE entry is the normalized name L"en-US".
 *        In the C locale the entry is NULL, empty or L"C". msvcr110/120 may store a different form (probe).
 */
#include "tc.h"
#include <locale.h>

wchar_t **__cdecl ___lc_locale_name_func(void);
static const char *volatile n_1252 = "English_United States.1252";
static const char *volatile n_c = "C";

TC_CASE(c_locale_entry_recorded)
{
    wchar_t **a = ___lc_locale_name_func();
    CHECK(a != NULL); if (!a) return;
    CHECK(a[LC_CTYPE] == NULL || tc_wcsequ(a[LC_CTYPE], L"") || tc_wcsequ(a[LC_CTYPE], L"C"));
}
TC_CASE(english_ctype_entry_en_US)
{
    wchar_t **a;
    if (!setlocale(LC_ALL, n_1252)) { setlocale(LC_ALL, n_c); TC_SKIP("English_United States.1252 unavailable"); }
    a = ___lc_locale_name_func();
    CHECK(a != NULL && a[LC_CTYPE] != NULL);
    if (a && a[LC_CTYPE]) CHECK_WSTR(a[LC_CTYPE], L"en-US");
    setlocale(LC_ALL, n_c);
}
int main(void)
{
    tc_begin();
    TC_RUN(c_locale_entry_recorded); TC_RUN(english_ctype_entry_en_US);
    setlocale(LC_ALL, "C");
    return tc_end();
}

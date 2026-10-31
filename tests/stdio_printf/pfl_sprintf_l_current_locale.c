/* TC
 * id:    pfl_sprintf_l_current_locale
 * area:  stdio_printf
 * funcs: _sprintf_l _get_current_locale _free_locale _create_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _get_current_locale, setlocale, sprintf/_sprintf_l
 * note:  setlocale(LC_NUMERIC, German) is restored to "C" at the end of each case. Skipped if German is not installed.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

static volatile double v_d = 2.5;
static const char * volatile v_c = "C";
static const char * volatile v_de = "German_Germany.1252";

TC_CASE(current_locale_nonnull)
{
    _locale_t l = _get_current_locale();
    CHECK(l != NULL);
    if (l) { char b[32]; CHECK_EQ_I(_sprintf_l(b, "%.1f", l, v_d), 3); CHECK_STR(b, "2.5"); _free_locale(l); }
}
TC_CASE(current_matches_sprintf)
{
    char a[32], b[32]; _locale_t l = _get_current_locale();
    if (!l) TC_SKIP("no current locale");
    sprintf(a, "%.1f", v_d); _sprintf_l(b, "%.1f", l, v_d);
    CHECK_STR(a, b);
    _free_locale(l);
}
TC_CASE(setlocale_de_affects_sprintf_not_l_c)
{
    char a[32], b[32]; _locale_t lc = _create_locale(LC_ALL, v_c);
    if (!lc) TC_SKIP("no C locale");
    if (!setlocale(LC_NUMERIC, v_de)) { _free_locale(lc); TC_SKIP("no German locale"); }
    sprintf(a, "%.1f", v_d);
    _sprintf_l(b, "%.1f", lc, v_d);
    setlocale(LC_ALL, v_c);
    CHECK_STR(a, "2,5");
    CHECK_STR(b, "2.5");
    _free_locale(lc);
}
TC_CASE(current_locale_tracks_setlocale)
{
    char b[32]; _locale_t l;
    if (!setlocale(LC_NUMERIC, v_de)) TC_SKIP("no German locale");
    l = _get_current_locale();
    setlocale(LC_ALL, v_c);
    if (!l) { CHECK(l != NULL); return; }
    _sprintf_l(b, "%.1f", l, v_d);
    CHECK_STR(b, "2,5");
    _free_locale(l);
}
TC_CASE(restore_c)
{
    char b[32];
    setlocale(LC_ALL, v_c);
    sprintf(b, "%.1f", v_d); CHECK_STR(b, "2.5");
}

int main(void)
{
    tc_begin();
    TC_RUN(current_locale_nonnull); TC_RUN(current_matches_sprintf); TC_RUN(setlocale_de_affects_sprintf_not_l_c);
    TC_RUN(current_locale_tracks_setlocale); TC_RUN(restore_c);
    return tc_end();
}

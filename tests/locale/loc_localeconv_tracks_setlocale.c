/* TC
 * id:    loc_localeconv_tracks_setlocale
 * area:  locale
 * funcs: setlocale localeconv
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs localeconv (LC_NUMERIC / LC_MONETARY)
 */
#include "tc.h"
#include <locale.h>

TC_CASE(numeric_only_changes_decimal_point)
{
    volatile const char *n = "German_Germany.1252";
    if (!setlocale(LC_NUMERIC, (const char *)n)) TC_SKIP("locale unavailable");
    CHECK_STR(localeconv()->decimal_point, ",");
    CHECK_STR(localeconv()->currency_symbol, "");
    setlocale(LC_ALL, "C");
}
TC_CASE(monetary_only_changes_currency_symbol)
{
    volatile const char *n = "English_United States.1252";
    if (!setlocale(LC_MONETARY, (const char *)n)) TC_SKIP("locale unavailable");
    CHECK_STR(localeconv()->currency_symbol, "$");
    CHECK_STR(localeconv()->decimal_point, ".");
    CHECK_STR(localeconv()->thousands_sep, "");
    setlocale(LC_ALL, "C");
}
TC_CASE(reset_to_C)
{
    volatile const char *n = "German_Germany.1252";
    setlocale(LC_ALL, (const char *)n);
    setlocale(LC_ALL, "C");
    CHECK_STR(localeconv()->decimal_point, ".");
    CHECK_STR(localeconv()->currency_symbol, "");
    CHECK_STR(setlocale(LC_ALL, NULL), "C");
}
int main(void) { tc_begin(); TC_RUN(numeric_only_changes_decimal_point); TC_RUN(monetary_only_changes_currency_symbol); TC_RUN(reset_to_C); return tc_end(); }

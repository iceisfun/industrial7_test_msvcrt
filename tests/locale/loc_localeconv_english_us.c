/* TC
 * id:    loc_localeconv_english_us
 * area:  locale
 * funcs: setlocale localeconv
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs localeconv, setlocale
 * note:  English_United States.1252 uses NLS system defaults for en-US; user overrides are ignored for an explicit locale name.
 */
#include "tc.h"
#include <locale.h>

static int enter(void) { volatile const char *n = "English_United States.1252"; return setlocale(LC_ALL, (const char *)n) != NULL; }

TC_CASE(numeric_group) { if (!enter()) TC_SKIP("locale unavailable"); CHECK_STR(localeconv()->decimal_point, "."); CHECK_STR(localeconv()->thousands_sep, ","); setlocale(LC_ALL, "C"); }
TC_CASE(grouping_3) { if (!enter()) TC_SKIP("locale unavailable"); CHECK_STR(localeconv()->grouping, "\3"); setlocale(LC_ALL, "C"); }
TC_CASE(currency_symbol) { if (!enter()) TC_SKIP("locale unavailable"); CHECK_STR(localeconv()->currency_symbol, "$"); CHECK_STR(localeconv()->negative_sign, "-"); CHECK_STR(localeconv()->positive_sign, ""); setlocale(LC_ALL, "C"); }
TC_CASE(frac_digits) { if (!enter()) TC_SKIP("locale unavailable"); CHECK_EQ_I(localeconv()->frac_digits, 2); CHECK_EQ_I(localeconv()->int_frac_digits, 2); setlocale(LC_ALL, "C"); }
TC_CASE(p_cs_precedes) { if (!enter()) TC_SKIP("locale unavailable"); CHECK_EQ_I(localeconv()->p_cs_precedes, 1); CHECK_EQ_I(localeconv()->p_sep_by_space, 0); setlocale(LC_ALL, "C"); }
TC_CASE(restore_C) { enter(); setlocale(LC_ALL, "C"); CHECK_STR(localeconv()->thousands_sep, ""); CHECK_STR(localeconv()->currency_symbol, ""); }
int main(void) { tc_begin(); TC_RUN(numeric_group); TC_RUN(grouping_3); TC_RUN(currency_symbol); TC_RUN(frac_digits); TC_RUN(p_cs_precedes); TC_RUN(restore_C); return tc_end(); }

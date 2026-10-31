/* TC
 * id:    loc_localeconv_german
 * area:  locale
 * funcs: setlocale localeconv
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs localeconv, setlocale
 * note:  Strings are read before the next setlocale call; the lconv is per-locale static.
 */
#include "tc.h"
#include <locale.h>

static int enter(void) { volatile const char *n = "German_Germany.1252"; return setlocale(LC_ALL, (const char *)n) != NULL; }

TC_CASE(decimal_comma) { if (!enter()) TC_SKIP("locale unavailable"); CHECK_STR(localeconv()->decimal_point, ","); setlocale(LC_ALL, "C"); }
TC_CASE(thousands_dot) { if (!enter()) TC_SKIP("locale unavailable"); CHECK_STR(localeconv()->thousands_sep, "."); setlocale(LC_ALL, "C"); }
TC_CASE(grouping) { if (!enter()) TC_SKIP("locale unavailable"); CHECK_STR(localeconv()->grouping, "\3"); setlocale(LC_ALL, "C"); }
TC_CASE(mon_decimal_comma) { if (!enter()) TC_SKIP("locale unavailable"); CHECK_STR(localeconv()->mon_decimal_point, ","); setlocale(LC_ALL, "C"); }
int main(void) { tc_begin(); TC_RUN(decimal_comma); TC_RUN(thousands_dot); TC_RUN(grouping); TC_RUN(mon_decimal_comma); return tc_end(); }

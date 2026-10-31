/* TC
 * id:    loc_neg_localeconv_after_failed_setlocale_german
 * area:  locale
 * funcs: localeconv setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs setlocale ("If the selection cannot be honored, returns NULL and the current locale settings of the program are not changed"); localeconv
 * note:  A failing setlocale must leave the lconv data of the last successfully set locale untouched. German case skipped if not installed.
 */
#include "tc.h"
#include <locale.h>

static const char *volatile v_bad = "zz";
static const char *volatile v_bad_all = "English_Nowhere.1252";
static const char *volatile v_de = "German_Germany.1252";

TC_CASE(from_c_locale)
{
    setlocale(LC_ALL, "C");
    CHECK(setlocale(LC_NUMERIC, v_bad) == NULL);
    CHECK_STR(localeconv()->decimal_point, ".");
    CHECK_STR(localeconv()->thousands_sep, "");
}
TC_CASE(german_then_fail)
{
    if (!setlocale(LC_ALL, v_de)) { setlocale(LC_ALL, "C"); TC_SKIP("German locale unavailable"); }
    CHECK(setlocale(LC_NUMERIC, v_bad) == NULL);
    CHECK_STR(localeconv()->decimal_point, ",");
    setlocale(LC_ALL, "C");
}
TC_CASE(fail_lc_all)
{
    if (!setlocale(LC_ALL, v_de)) { setlocale(LC_ALL, "C"); TC_SKIP("German locale unavailable"); }
    CHECK(setlocale(LC_ALL, v_bad_all) == NULL);
    CHECK_STR(localeconv()->decimal_point, ",");
    setlocale(LC_ALL, "C");
}
TC_CASE(pointer_non_null_after_failure)
{
    setlocale(LC_ALL, "C");
    setlocale(LC_ALL, v_bad_all);
    CHECK(localeconv() != NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(from_c_locale); TC_RUN(german_then_fail); TC_RUN(fail_lc_all); TC_RUN(pointer_non_null_after_failure);
    setlocale(LC_ALL, "C");
    return tc_end();
}

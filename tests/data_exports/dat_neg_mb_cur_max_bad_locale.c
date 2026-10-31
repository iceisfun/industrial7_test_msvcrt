/* TC
 * id:    dat_neg_mb_cur_max_bad_locale
 * area:  data_exports
 * funcs: ___mb_cur_max_func setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs setlocale ("If the locale cannot be set, returns NULL and the current locale is unchanged")
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

TC_CASE(bogus_name_fails_and_keeps_c)
{
    CHECK(setlocale(LC_ALL, "No_Such_Locale_Xyz_Qq") == NULL);
    CHECK_STR(setlocale(LC_ALL, NULL), "C");
    CHECK_EQ_I(___mb_cur_max_func(), 1);
}
TC_CASE(bogus_ctype_fails_and_keeps_c)
{
    CHECK(setlocale(LC_CTYPE, "zz_ZZ_not_a_locale.1252") == NULL);
    CHECK_STR(setlocale(LC_CTYPE, NULL), "C");
    CHECK_EQ_I(___mb_cur_max_func(), 1);
}
int main(void) { tc_begin(); TC_RUN(bogus_name_fails_and_keeps_c); TC_RUN(bogus_ctype_fails_and_keeps_c); setlocale(LC_ALL, "C"); return tc_end(); }

/* TC
 * id:    loc_setlocale_bcp47_ucrt
 * area:  locale
 * funcs: setlocale localeconv
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs setlocale (UCRT: locale names in BCP-47 form)
 */
#include "tc.h"
#include <locale.h>

TC_CASE(en_US_nonnull) { volatile const char *n = "en-US"; CHECK(setlocale(LC_ALL, (const char *)n) != NULL); setlocale(LC_ALL, "C"); }
TC_CASE(fr_FR_nonnull) { volatile const char *n = "fr-FR"; CHECK(setlocale(LC_ALL, (const char *)n) != NULL); setlocale(LC_ALL, "C"); }
TC_CASE(fr_decimal_comma)
{
    volatile const char *n = "fr-FR";
    if (!setlocale(LC_ALL, (const char *)n)) TC_SKIP("fr-FR rejected");
    CHECK_STR(localeconv()->decimal_point, ",");
    setlocale(LC_ALL, "C");
    CHECK_STR(localeconv()->decimal_point, ".");
}
int main(void) { tc_begin(); TC_RUN(en_US_nonnull); TC_RUN(fr_FR_nonnull); TC_RUN(fr_decimal_comma); return tc_end(); }

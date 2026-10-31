/* TC
 * id:    ct_cp1252_classification
 * area:  ctype
 * funcs: isalpha isupper islower setlocale
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs setlocale (language_country.codepage)
 * note:  TC_SKIP if the locale is not installed. Only letter classes; ispunct/C1 guesses live in ct_probe_cp1252_punct_c1.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

static int have;
TC_CASE(setlocale_ok_or_skip) { have = setlocale(LC_ALL, "English_United States.1252") != NULL; if (!have) TC_SKIP("locale unavailable"); }
TC_CASE(latin1_letters)
{
    volatile int c;
    if (!have) TC_SKIP("no locale");
    for (c = 0xC0; c <= 0xDE; c++) if (c != 0xD7) { CHECK(isalpha(c)); CHECK(isupper(c)); CHECK(!islower(c)); }
    for (c = 0xDF; c <= 0xFF; c++) if (c != 0xF7) { CHECK(isalpha(c)); CHECK(islower(c)); CHECK(!isupper(c)); }
}
TC_CASE(multiply_divide_not_alpha)
{
    volatile int c;
    if (!have) TC_SKIP("no locale");
    c = 0xD7; CHECK(!isalpha(c));
    c = 0xF7; CHECK(!isalpha(c));
}
TC_CASE(restore_C_locale)
{
    CHECK(setlocale(LC_ALL, "C") != NULL);
    { volatile int c = 0xE9; CHECK(!isalpha(c)); CHECK(!islower(c)); }
}
int main(void) { tc_begin(); TC_RUN(setlocale_ok_or_skip); TC_RUN(latin1_letters); TC_RUN(multiply_divide_not_alpha); TC_RUN(restore_C_locale); return tc_end(); }

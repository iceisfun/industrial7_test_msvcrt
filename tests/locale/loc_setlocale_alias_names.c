/* TC
 * id:    loc_setlocale_alias_names
 * area:  locale
 * funcs: setlocale
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs setlocale (language and country strings)
 * note:  PROBE: language_country forms English_USA, english-usa (dash separator), German_Germany are accepted. Documented table lists language 'english' and country 'usa'; the dash form is a guess.
 */
#include "tc.h"
#include <locale.h>

TC_CASE(english_usa_nonnull) { volatile const char *n = "English_USA"; CHECK(setlocale(LC_ALL, (const char *)n) != NULL); setlocale(LC_ALL, "C"); }
TC_CASE(english_dash_usa_nonnull) { volatile const char *n = "english-usa"; CHECK(setlocale(LC_ALL, (const char *)n) != NULL); setlocale(LC_ALL, "C"); }
TC_CASE(german_germany_nonnull) { volatile const char *n = "German_Germany"; CHECK(setlocale(LC_ALL, (const char *)n) != NULL); setlocale(LC_ALL, "C"); }
int main(void) { tc_begin(); TC_RUN(english_usa_nonnull); TC_RUN(english_dash_usa_nonnull); TC_RUN(german_germany_nonnull); return tc_end(); }

/* TC
 * id:    loc_neg_create_locale_invalid_name
 * area:  locale
 * funcs: _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _create_locale (returns NULL when the locale cannot be created); setlocale (invalid names)
 */
#include "tc.h"
#include <locale.h>

static const char *volatile n_garbage = "zz_nowhere";
static const char *volatile n_country = "English_Nowhere";

TC_CASE(garbage_null) { _locale_t l = _create_locale(LC_ALL, n_garbage); CHECK(l == NULL); if (l) _free_locale(l); }
TC_CASE(unknown_country_null) { _locale_t l = _create_locale(LC_CTYPE, n_country); CHECK(l == NULL); if (l) _free_locale(l); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(garbage_null); TC_RUN(unknown_country_null); return tc_end(); }

/* TC
 * id:    loc_create_free_locale_basic
 * area:  locale
 * funcs: _create_locale _free_locale _get_current_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _create_locale, _wcreate_locale; _free_locale; _get_current_locale
 * note:  _free_locale(NULL) is covered separately in loc_neg_free_locale_null.
 */
#include "tc.h"
#include <locale.h>

static const char *volatile n_c = "C";
static const char *volatile n_1252 = "English_United States.1252";

TC_CASE(create_C) { _locale_t l = _create_locale(LC_ALL, n_c); CHECK(l != NULL); if (l) _free_locale(l); }
TC_CASE(create_english)
{
    _locale_t l = _create_locale(LC_ALL, n_1252);
    if (!l) TC_SKIP("English_United States.1252 unavailable");
    CHECK(l != NULL); _free_locale(l);
}
TC_CASE(free_ok)
{
    _locale_t l = _create_locale(LC_CTYPE, n_c);
    CHECK(l != NULL);
    _free_locale(l);
}
TC_CASE(get_current_nonnull_and_free)
{
    _locale_t l = _get_current_locale();
    CHECK(l != NULL);
    if (l) _free_locale(l);
}
int main(void)
{
    tc_begin();
    TC_RUN(create_C); TC_RUN(create_english); TC_RUN(free_ok); TC_RUN(get_current_nonnull_and_free);
    return tc_end();
}

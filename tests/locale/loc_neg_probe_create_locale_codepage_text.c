/* TC
 * id:    loc_neg_probe_create_locale_codepage_text
 * area:  locale
 * funcs: _create_locale _free_locale setlocale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _create_locale (locale name grammar)
 * note:  PROBE: a non-numeric code page suffix ("abc") or numeric prefix with trailing garbage ("1252x") is believed rejected (NULL).
 *        The CRT may parse the suffix with an atol-like routine, in which case "1252x" could be accepted; not documented.
 */
#include "tc.h"
#include <locale.h>

static const char *volatile v_n;
TC_CASE(abc) { _locale_t l; v_n = "English_United States.abc"; l = _create_locale(LC_ALL, v_n); CHECK(l == NULL); if (l) _free_locale(l); }
TC_CASE(trailing_x) { _locale_t l; v_n = ".1252x"; l = _create_locale(LC_ALL, v_n); CHECK(l == NULL); if (l) _free_locale(l); }
int main(void)
{
    tc_begin();
    TC_RUN(abc); TC_RUN(trailing_x);
    setlocale(LC_ALL, "C");
    return tc_end();
}

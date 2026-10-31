/* TC
 * id:    loc_neg_create_locale_bad_codepage_text
 * area:  locale
 * funcs: _create_locale _free_locale setlocale
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _create_locale, setlocale (code page must be valid and supported; UTF-7/UTF-8 note: CP_UTF7 not supported)
 * note:  Non-numeric / trailing-garbage code page text moved to loc_neg_probe_create_locale_codepage_text (not documented).
 */
#include "tc.h"
#include <locale.h>

static const char *volatile v_n;
TC_CASE(cp99999) { _locale_t l; v_n = ".99999"; l = _create_locale(LC_ALL, v_n); CHECK(l == NULL); if (l) _free_locale(l); }
TC_CASE(utf7) { _locale_t l; v_n = ".65000"; l = _create_locale(LC_ALL, v_n); CHECK(l == NULL); if (l) _free_locale(l); }
TC_CASE(global_c) { CHECK_STR(setlocale(LC_ALL, NULL), "C"); }
int main(void)
{
    tc_begin();
    TC_RUN(cp99999); TC_RUN(utf7); TC_RUN(global_c);
    return tc_end();
}

/* TC
 * id:    loc_neg_probe_create_locale_codepage_text
 * area:  locale
 * funcs: _create_locale _free_locale setlocale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _create_locale (locale name grammar)
 * note:  Win10 triage (class A, probe -> observed): "English_United States.abc" is rejected, ".1252x" is accepted by ucrtbase 19045 (observed on Windows
 *        10 22H2 build 19045).
 *        Original PROBE: a non-numeric code page suffix ("abc") or numeric prefix with trailing garbage ("1252x") is believed rejected (NULL).
 *        The CRT may parse the suffix with an atol-like routine, in which case "1252x" could be accepted; not documented.
 *        [scope] observed on ucrtbase 19045 only; the legacy CRTs listed in crt: are [unverified] until the P3 runs.
 *        [env] user-default locale en-US, build 19045; not stable on a VM with another user locale/NLS data.
 */
#include "tc.h"
#include <locale.h>

static const char *volatile v_n;
TC_CASE(abc) { _locale_t l; v_n = "English_United States.abc"; l = _create_locale(LC_ALL, v_n); CHECK(l == NULL); if (l) _free_locale(l); }
/* observed on Win10 22H2 ucrtbase: trailing junk after the numeric code page is ignored (atoi-like), so the locale object IS created */
TC_CASE(trailing_x) { _locale_t l; v_n = ".1252x"; l = _create_locale(LC_ALL, v_n); CHECK(l != NULL); if (l) _free_locale(l); }
int main(void)
{
    tc_begin();
    TC_RUN(abc); TC_RUN(trailing_x);
    setlocale(LC_ALL, "C");
    return tc_end();
}

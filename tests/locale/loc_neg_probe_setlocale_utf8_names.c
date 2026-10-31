/* TC
 * id:    loc_neg_probe_setlocale_utf8_names
 * area:  locale
 * funcs: setlocale _create_locale _free_locale
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs UTF-8 support in the UCRT (setlocale, Windows 10 1803+): ".UTF-8" / ".utf8" / ".65001" are valid code page suffixes
 * note:  PROBE: malformed UTF-8 locale names are believed to be rejected (NULL) by both setlocale and _create_locale (parity).
 *        ".utf8", "C.UTF-8" are NOT asserted (printed only). Each name is its own case. Restores "C".
 */
#include "tc.h"
#include <locale.h>

static const char *volatile v_n;

static void parity(const char *name, int want_null)
{
    const char *r; _locale_t l; v_n = name;
    r = setlocale(LC_ALL, v_n);
    l = _create_locale(LC_ALL, v_n);
    if (want_null) CHECK(r == NULL);
    CHECK((r == NULL) == (l == NULL));
    if (l) _free_locale(l);
    setlocale(LC_ALL, "C");
}
TC_CASE(utf8x) { parity(".UTF-8x", 1); }
TC_CASE(english_utf8_dot1) { parity("English_United States.UTF-8.1", 1); }
TC_CASE(cp65001x) { parity(".65001x", 1); }
TC_CASE(en_us_utf8_dot1) { parity("en-US.UTF-8.1", 1); }
TC_CASE(diag_lenient_names)
{
    static const char *const n[] = { ".utf8", "C.UTF-8", ".UTF-8", ".65001" };
    int i;
    for (i = 0; i < 4; i++) {
        const char *r; _locale_t l; v_n = n[i];
        r = setlocale(LC_ALL, v_n); l = _create_locale(LC_ALL, v_n);
        tc_write("TC INFO name "); tc_write(n[i]); tc_write(r ? " accepted\n" : " rejected\n");
        CHECK((r == NULL) == (l == NULL));
        if (l) _free_locale(l);
        setlocale(LC_ALL, "C");
    }
}
int main(void)
{
    tc_begin();
    TC_RUN(utf8x); TC_RUN(english_utf8_dot1); TC_RUN(cp65001x); TC_RUN(en_us_utf8_dot1); TC_RUN(diag_lenient_names);
    setlocale(LC_ALL, "C");
    return tc_end();
}

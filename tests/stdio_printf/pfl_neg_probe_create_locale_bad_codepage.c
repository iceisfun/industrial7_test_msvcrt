/* TC
 * id:    pfl_neg_probe_create_locale_bad_codepage
 * area:  stdio_printf
 * kind:  negative
 * funcs: _create_locale _free_locale
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _create_locale, setlocale (locale name syntax locale[.code_page]; NULL on failure)
 * note:  PROBE: malformed or unsupported code page suffixes make _create_locale return NULL. The specific strings
 *        (".99999", "en-US.99999", "C.", "..1252") are believed invalid; each is its own case so one wrong guess is isolated.
 *        The final case (plain "C") is documented.
 */
#include "tc.h"
#include <locale.h>

static const char * volatile v_name;

static void bad(const char *n)
{
    _locale_t l; v_name = n;
    l = _create_locale(LC_ALL, v_name);
    CHECK(l == NULL);
    if (l) _free_locale(l);
}
TC_CASE(dot_99999) { bad(".99999"); }
TC_CASE(en_us_99999) { bad("en-US.99999"); }
TC_CASE(c_dot_empty) { bad("C."); }
TC_CASE(double_dot) { bad("..1252"); }
TC_CASE(c_still_works)
{
    _locale_t l = _create_locale(LC_ALL, "C");
    CHECK(l != NULL);
    if (l) _free_locale(l);
}

int main(void)
{
    tc_begin();
    TC_RUN(dot_99999); TC_RUN(en_us_99999); TC_RUN(c_dot_empty); TC_RUN(double_dot); TC_RUN(c_still_works);
    return tc_end();
}

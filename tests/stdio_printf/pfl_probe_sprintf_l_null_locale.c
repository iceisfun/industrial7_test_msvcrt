/* TC
 * id:    pfl_probe_sprintf_l_null_locale
 * area:  stdio_printf
 * funcs: _sprintf_l
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _sprintf_l (locale parameter); CRT _LocaleUpdate convention
 * note:  PROBE: a NULL _locale_t is accepted by _sprintf_l and means "current locale", without calling the invalid-parameter handler.
 *        Docs do not state it. Failure here would indicate NULL is rejected or crashes (a crash counts as fail).
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

static volatile double v_d = 2.5;
static const char * volatile v_c = "C";
static const char * volatile v_de = "German_Germany.1252";

TC_CASE(null_locale_return_len)
{
    char b[32]; int n;
    tc_iph_install(); tc_iph_count = 0;
    n = _sprintf_l(b, "ab%d", NULL, 12);
    CHECK_EQ_I(n, 4); CHECK_STR(b, "ab12");
}
TC_CASE(null_locale_matches_sprintf_in_c)
{
    char a[32], b[32];
    sprintf(a, "%.1f", v_d); _sprintf_l(b, "%.1f", NULL, v_d);
    CHECK_STR(a, "2.5"); CHECK_STR(b, "2.5");
}
TC_CASE(null_locale_under_german_setlocale_uses_comma)
{
    char b[32];
    if (!setlocale(LC_NUMERIC, v_de)) TC_SKIP("no German locale");
    _sprintf_l(b, "%.1f", NULL, v_d);
    setlocale(LC_ALL, v_c);
    CHECK_STR(b, "2,5");
}
TC_CASE(iph_not_called) { CHECK_EQ_I(tc_iph_count, 0); }

int main(void)
{
    tc_begin();
    TC_RUN(null_locale_return_len); TC_RUN(null_locale_matches_sprintf_in_c);
    TC_RUN(null_locale_under_german_setlocale_uses_comma); TC_RUN(iph_not_called);
    return tc_end();
}

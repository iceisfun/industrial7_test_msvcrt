/* TC
 * id:    pfl_sprintf_l_german_decimal
 * area:  stdio_printf
 * funcs: _sprintf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf, _sprintf_l (locale-dependent decimal point), _create_locale
 * note:  Skipped when German_Germany.1252 is not installed. %a is covered separately by a probe.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

static volatile double v_d;
static const char * volatile v_c = "C";
static const char * volatile v_de = "German_Germany.1252";

TC_CASE(f_comma)
{
    char b[64]; _locale_t l = _create_locale(LC_ALL, v_de);
    if (!l) TC_SKIP("no German locale");
    v_d = 3.5; _sprintf_l(b, "%f", l, v_d); CHECK_STR(b, "3,500000");
    v_d = 0.25; _sprintf_l(b, "%.2f", l, v_d); CHECK_STR(b, "0,25");
    _free_locale(l);
}
TC_CASE(e_comma)
{
    char b[64]; _locale_t l = _create_locale(LC_ALL, v_de);
    if (!l) TC_SKIP("no German locale");
    v_d = 1234.5; _sprintf_l(b, "%e", l, v_d);
#ifdef TC_UCRT
    CHECK_STR(b, "1,234500e+03");
#else
    CHECK(tc_strequ(b, "1,234500e+03") || tc_strequ(b, "1,234500e+003"));
#endif
    _free_locale(l);
}
TC_CASE(g_comma)
{
    char b[64]; _locale_t l = _create_locale(LC_ALL, v_de);
    if (!l) TC_SKIP("no German locale");
    v_d = 0.5; _sprintf_l(b, "%g", l, v_d); CHECK_STR(b, "0,5");
    v_d = 12.75; _sprintf_l(b, "%g", l, v_d); CHECK_STR(b, "12,75");
    _free_locale(l);
}
TC_CASE(same_buffer_c_vs_de_differ)
{
    char a[64], b[64]; _locale_t lc = _create_locale(LC_ALL, v_c), ld = _create_locale(LC_ALL, v_de);
    if (!lc || !ld) { if (lc) _free_locale(lc); if (ld) _free_locale(ld); TC_SKIP("locale missing"); }
    v_d = 9.5;
    _sprintf_l(a, "%.1f", lc, v_d);
    _sprintf_l(b, "%.1f", ld, v_d);
    CHECK_STR(a, "9.5"); CHECK_STR(b, "9,5");
    CHECK(!tc_strequ(a, b));
    _free_locale(lc); _free_locale(ld);
}
TC_CASE(integers_unaffected)
{
    char b[64]; _locale_t l = _create_locale(LC_ALL, v_de);
    if (!l) TC_SKIP("no German locale");
    _sprintf_l(b, "%d|%x", l, 12345, 255); CHECK_STR(b, "12345|ff");
    _free_locale(l);
}

int main(void)
{
    tc_begin();
    TC_RUN(f_comma); TC_RUN(e_comma); TC_RUN(g_comma); TC_RUN(same_buffer_c_vs_de_differ); TC_RUN(integers_unaffected);
    return tc_end();
}

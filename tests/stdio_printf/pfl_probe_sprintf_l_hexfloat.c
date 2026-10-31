/* TC
 * id:    pfl_probe_sprintf_l_hexfloat
 * area:  stdio_printf
 * funcs: _sprintf_l _create_locale _free_locale
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.1 (%a); MS docs printf type field characters
 * note:  PROBE: ucrt %a for 0.5 gives "0x1.0000000000000p-1" (default precision 13, lowercase) in the C locale, and whether the German
 *        locale substitutes ',' for the hex-float radix point. Kept separate from documented checks.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

static volatile double v_d;
static const char * volatile v_c = "C";
static const char * volatile v_de = "German_Germany.1252";

TC_CASE(a_c_locale)
{
    char b[64]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l) TC_SKIP("no C locale");
    v_d = 0.5; _sprintf_l(b, "%a", l, v_d); CHECK_STR(b, "0x1.0000000000000p-1");
    _free_locale(l);
}
TC_CASE(a_german_comma)
{
    char b[64]; _locale_t l = _create_locale(LC_ALL, v_de);
    if (!l) TC_SKIP("no German locale");
    v_d = 0.5; _sprintf_l(b, "%a", l, v_d); CHECK_STR(b, "0x1,0000000000000p-1");
    _free_locale(l);
}

int main(void) { tc_begin(); TC_RUN(a_c_locale); TC_RUN(a_german_comma); return tc_end(); }

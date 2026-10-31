/* TC
 * id:    loc_atof_strtod_l_decimal_point
 * area:  locale
 * funcs: _atof_l _strtod_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strtod, _strtod_l, atof, _atof_l
 * note:  Decimal point follows the locale argument, not the global locale. German locale skipped if unavailable.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static _locale_t de, lc;
static const char *volatile v_comma = "1,5";
static const char *volatile v_dot = "1.5";

TC_CASE(german_comma)
{
    if (!de) TC_SKIP("German locale unavailable");
    CHECK_BITS_D(_atof_l(v_comma, de), 1.5);
    CHECK_BITS_D(_strtod_l(v_comma, NULL, de), 1.5);
}
TC_CASE(german_endptr)
{
    char *e = NULL; double d;
    if (!de) TC_SKIP("German locale unavailable");
    d = _strtod_l(v_comma, &e, de);
    CHECK_BITS_D(d, 1.5); CHECK_EQ_P(e, v_comma + 3);
}
TC_CASE(c_dot)
{
    char *e = NULL;
    CHECK_BITS_D(_strtod_l(v_dot, &e, lc), 1.5); CHECK_EQ_P(e, v_dot + 3);
    CHECK_BITS_D(_atof_l(v_dot, lc), 1.5);
}
TC_CASE(c_comma_stops)
{
    char *e = NULL;
    CHECK_BITS_D(_strtod_l(v_comma, &e, lc), 1.0); CHECK_EQ_P(e, v_comma + 1);
}
TC_CASE(german_dot_stops)
{
    char *e = NULL;
    if (!de) TC_SKIP("German locale unavailable");
    CHECK_BITS_D(_strtod_l(v_dot, &e, de), 1.0); CHECK_EQ_P(e, v_dot + 1);
}
int main(void)
{
    tc_begin();
    de = _create_locale(LC_ALL, "German_Germany.1252");
    lc = _create_locale(LC_ALL, "C");
    if (!lc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(german_comma); TC_RUN(german_endptr); TC_RUN(c_dot); TC_RUN(c_comma_stops); TC_RUN(german_dot_stops);
    if (de) _free_locale(de);
    _free_locale(lc);
    return tc_end();
}

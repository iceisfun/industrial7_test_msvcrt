/* TC
 * id:    loc_neg_strtod_l_no_grouping
 * area:  locale
 * funcs: _strtod_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.3 (no grouping in subject sequence)
 * note:  Thousands separators are never accepted by strtod.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static _locale_t de, lc;
static const char *volatile v_c = "1,234";
static const char *volatile v_de = "1.234,5";

TC_CASE(c_comma_not_grouping)
{
    char *e = NULL;
    CHECK_BITS_D(_strtod_l(v_c, &e, lc), 1.0); CHECK_EQ_P(e, v_c + 1);
}
TC_CASE(german_dot_not_grouping)
{
    char *e = NULL;
    if (!de) TC_SKIP("German locale unavailable");
    CHECK_BITS_D(_strtod_l(v_de, &e, de), 1.0);
}
TC_CASE(endptr_positions)
{
    char *e = NULL;
    if (!de) TC_SKIP("German locale unavailable");
    _strtod_l(v_de, &e, de); CHECK_EQ_P(e, v_de + 1);
}
int main(void)
{
    tc_begin();
    de = _create_locale(LC_ALL, "German_Germany.1252");
    lc = _create_locale(LC_ALL, "C");
    if (!lc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(c_comma_not_grouping); TC_RUN(german_dot_not_grouping); TC_RUN(endptr_positions);
    if (de) _free_locale(de);
    _free_locale(lc);
    return tc_end();
}

/* TC
 * id:    pfl_create_locale_c
 * area:  stdio_printf
 * funcs: _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _create_locale, _wcreate_locale; _free_locale
 * note:  _create_locale returns NULL for an invalid category or locale name; _free_locale(NULL) is documented as a no-op.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

static const char * volatile v_c = "C";
static const char * volatile v_bogus = "no_such_locale_xyz_123";

TC_CASE(c_locale_nonnull)
{
    _locale_t l = _create_locale(LC_ALL, v_c);
    CHECK(l != NULL);
    _free_locale(l);
}
TC_CASE(free_null_ok) { _free_locale(NULL); CHECK(1); }
TC_CASE(create_same_name_twice_distinct_or_valid)
{
    _locale_t a = _create_locale(LC_ALL, v_c), b = _create_locale(LC_ALL, v_c);
    CHECK(a != NULL);
    CHECK(b != NULL);
    _free_locale(a);
    _free_locale(b);
}
TC_CASE(lc_numeric_c_nonnull)
{
    _locale_t l = _create_locale(LC_NUMERIC, v_c);
    CHECK(l != NULL);
    _free_locale(l);
}
TC_CASE(bogus_name_returns_null)
{
    _locale_t l = _create_locale(LC_ALL, v_bogus);
    CHECK(l == NULL);
    if (l) _free_locale(l);
}

int main(void)
{
    tc_begin();
    TC_RUN(c_locale_nonnull); TC_RUN(free_null_ok); TC_RUN(create_same_name_twice_distinct_or_valid);
    TC_RUN(lc_numeric_c_nonnull); TC_RUN(bogus_name_returns_null);
    return tc_end();
}

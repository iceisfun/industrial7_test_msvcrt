/* TC
 * id:    pfl_neg_create_locale_invalid
 * area:  stdio_printf
 * kind:  negative
 * funcs: _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _create_locale: returns a locale object, or NULL for an invalid category or locale name; _free_locale
 * note:  Whether the invalid-parameter handler runs is not asserted (a counting handler is installed only so a fatal default cannot
 *        end the process). _free_locale(NULL) is documented as a no-op.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <locale.h>

TC_CASE(bad_category_neg1)
{
    volatile int cat = -1; _locale_t l = _create_locale(cat, "C");
    CHECK(l == NULL); if (l) _free_locale(l);
}
TC_CASE(bad_category_99)
{
    volatile int cat = 99; _locale_t l = _create_locale(cat, "C");
    CHECK(l == NULL); if (l) _free_locale(l);
}
TC_CASE(unknown_name_null)
{
    const char *volatile n = "No_Such_Locale"; _locale_t l = _create_locale(LC_ALL, n);
    CHECK(l == NULL); if (l) _free_locale(l);
}
TC_CASE(free_locale_null_no_crash)
{
    _locale_t volatile l = NULL;
    _free_locale(l);
    CHECK(1);
}
TC_CASE(valid_still_works_after_failures)
{
    _locale_t l = _create_locale(LC_ALL, "C");
    CHECK(l != NULL); if (l) _free_locale(l);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(bad_category_neg1); TC_RUN(bad_category_99); TC_RUN(unknown_name_null);
    TC_RUN(free_locale_null_no_crash); TC_RUN(valid_still_works_after_failures);
    return tc_end();
}

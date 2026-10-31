/* TC
 * id:    loc_neg_localeconv_c_unspecified_fields
 * area:  locale
 * funcs: localeconv
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.11.2.1 (C locale lconv table: "" and CHAR_MAX mean not available); MS docs localeconv
 * note:  In the "C" locale every monetary member is "not available": empty strings and CHAR_MAX.
 */
#include "tc.h"
#include <locale.h>
#include <limits.h>

TC_CASE(monetary_strings_empty)
{
    struct lconv *l; setlocale(LC_ALL, "C"); l = localeconv();
    CHECK_STR(l->int_curr_symbol, ""); CHECK_STR(l->currency_symbol, "");
    CHECK_STR(l->mon_decimal_point, ""); CHECK_STR(l->mon_thousands_sep, "");
    CHECK_STR(l->mon_grouping, ""); CHECK_STR(l->positive_sign, ""); CHECK_STR(l->negative_sign, "");
}
TC_CASE(monetary_chars_char_max)
{
    struct lconv *l; setlocale(LC_ALL, "C"); l = localeconv();
    CHECK_EQ_I(l->int_frac_digits, CHAR_MAX); CHECK_EQ_I(l->frac_digits, CHAR_MAX);
    CHECK_EQ_I(l->p_cs_precedes, CHAR_MAX); CHECK_EQ_I(l->p_sep_by_space, CHAR_MAX);
    CHECK_EQ_I(l->n_cs_precedes, CHAR_MAX); CHECK_EQ_I(l->n_sep_by_space, CHAR_MAX);
    CHECK_EQ_I(l->p_sign_posn, CHAR_MAX); CHECK_EQ_I(l->n_sign_posn, CHAR_MAX);
}
TC_CASE(no_grouping)
{
    struct lconv *l; setlocale(LC_ALL, "C"); l = localeconv();
    CHECK_STR(l->grouping, ""); CHECK_STR(l->thousands_sep, "");
}
TC_CASE(decimal_point_dot) { setlocale(LC_ALL, "C"); CHECK_STR(localeconv()->decimal_point, "."); }
int main(void)
{
    tc_begin();
    TC_RUN(monetary_strings_empty); TC_RUN(monetary_chars_char_max); TC_RUN(no_grouping); TC_RUN(decimal_point_dot);
    return tc_end();
}

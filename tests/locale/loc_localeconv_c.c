/* TC
 * id:    loc_localeconv_c
 * area:  locale
 * funcs: localeconv
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.11.2.1; MS docs localeconv
 */
#include "tc.h"
#include <locale.h>
#include <limits.h>

TC_CASE(nonnull_result) { CHECK(localeconv() != NULL); }
TC_CASE(decimal_point) { CHECK_STR(localeconv()->decimal_point, "."); }
TC_CASE(string_members_empty_thousands_grouping_currency_intcurr_mondp_monts_mongroup_possign_negsign)
{
    struct lconv *l = localeconv();
    CHECK_STR(l->thousands_sep, ""); CHECK_STR(l->grouping, ""); CHECK_STR(l->currency_symbol, "");
    CHECK_STR(l->int_curr_symbol, ""); CHECK_STR(l->mon_decimal_point, ""); CHECK_STR(l->mon_thousands_sep, "");
    CHECK_STR(l->mon_grouping, ""); CHECK_STR(l->positive_sign, ""); CHECK_STR(l->negative_sign, "");
}
TC_CASE(char_max_members_int_frac_digits_frac_digits_p_cs_precedes_p_sep_by_space_n_cs_precedes_n_sep_by_space_p_sign_posn_n_sign_posn)
{
    struct lconv *l = localeconv();
    CHECK_EQ_I(l->int_frac_digits, CHAR_MAX); CHECK_EQ_I(l->frac_digits, CHAR_MAX);
    CHECK_EQ_I(l->p_cs_precedes, CHAR_MAX); CHECK_EQ_I(l->p_sep_by_space, CHAR_MAX);
    CHECK_EQ_I(l->n_cs_precedes, CHAR_MAX); CHECK_EQ_I(l->n_sep_by_space, CHAR_MAX);
    CHECK_EQ_I(l->p_sign_posn, CHAR_MAX); CHECK_EQ_I(l->n_sign_posn, CHAR_MAX);
}
int main(void)
{
    tc_begin(); setlocale(LC_ALL, "C");
    TC_RUN(nonnull_result); TC_RUN(decimal_point);
    TC_RUN(string_members_empty_thousands_grouping_currency_intcurr_mondp_monts_mongroup_possign_negsign);
    TC_RUN(char_max_members_int_frac_digits_frac_digits_p_cs_precedes_p_sep_by_space_n_cs_precedes_n_sep_by_space_p_sign_posn_n_sign_posn);
    return tc_end();
}

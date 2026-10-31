/* TC
 * id:    ct_neg_wctype_wctrans_names_not_interchangeable
 * area:  ctype
 * kind:  negative
 * funcs:
 * procs: wctype wctrans
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.30.2.2.2 wctype, 7.30.3.2.2 wctrans; MS docs wctype, wctrans
 */
#include "tc.h"

typedef unsigned short (__cdecl *wct_p)(const char *);
static wct_p p_wctype, p_wctrans;
static const char *volatile n_alpha = "alpha", *volatile n_upper = "upper", *volatile n_lower = "lower";
static const char *volatile n_toupper = "toupper", *volatile n_tolower = "tolower";

TC_CASE(class_names_are_not_trans_names)
{
    if (!p_wctrans) TC_SKIP("missing");
    CHECK_EQ_U(p_wctrans(n_alpha), 0);
    CHECK_EQ_U(p_wctrans(n_upper), 0);
    CHECK_EQ_U(p_wctrans(n_lower), 0);
}
TC_CASE(trans_names_are_not_class_names)
{
    if (!p_wctype) TC_SKIP("missing");
    CHECK_EQ_U(p_wctype(n_toupper), 0);
    CHECK_EQ_U(p_wctype(n_tolower), 0);
}
TC_CASE(sanity_valid_names)
{
    if (!p_wctype || !p_wctrans) TC_SKIP("missing");
    CHECK(p_wctype(n_alpha) != 0);
    CHECK(p_wctrans(n_toupper) != 0);
    CHECK(p_wctrans(n_tolower) != 0);
}
int main(void)
{
    tc_begin();
    p_wctype = (wct_p)tc_crt_proc("wctype"); p_wctrans = (wct_p)tc_crt_proc("wctrans");
    TC_RUN(class_names_are_not_trans_names); TC_RUN(trans_names_are_not_class_names); TC_RUN(sanity_valid_names);
    return tc_end();
}

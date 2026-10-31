/* TC
 * id:    ct_neg_probe_wctype_whitespace_names
 * area:  ctype
 * funcs:
 * procs: wctype wctrans
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.30.2.2.2 (wctype: only the defined property names), 7.30.3.2.2 (wctrans); MS docs wctype
 * note:  Names are matched exactly; padded, mixed-case and near-miss names return 0.
 */
#include "tc.h"

typedef unsigned short (__cdecl *wc_t)(const char *);
static wc_t p_wctype, p_wctrans;

TC_CASE(wctype_bad)
{
    CHECK_EQ_U(p_wctype(" alpha"), 0);
    CHECK_EQ_U(p_wctype("alpha "), 0);
    CHECK_EQ_U(p_wctype("digit\n"), 0);
    CHECK_EQ_U(p_wctype("Alpha"), 0);
}
TC_CASE(wctrans_bad)
{
    CHECK_EQ_U(p_wctrans("tolower "), 0);
    CHECK_EQ_U(p_wctrans("to_upper"), 0);
}
TC_CASE(sanity) { CHECK(p_wctype("upper") != 0); }
int main(void)
{
    tc_begin();
    p_wctype = (wc_t)tc_crt_proc("wctype"); p_wctrans = (wc_t)tc_crt_proc("wctrans");
    if (!p_wctype || !p_wctrans) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(wctype_bad); TC_RUN(wctrans_bad); TC_RUN(sanity);
    return tc_end();
}

/* TC
 * id:    ct_neg_wctype_wctrans_unknown
 * area:  ctype
 * funcs: 
 * procs: wctype wctrans
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.30.2.2.2 wctype, 7.30.3.2 wctrans: unrecognized property/mapping returns zero
 * note:  Unknown names return 0. (iswctype with descriptor 0 is unspecified by C; see ct_probe_iswctype_zero_desc.)
 */
#include "tc.h"
#include <wctype.h>

typedef wctype_t (__cdecl *wctype_f)(const char *);
typedef wctrans_t (__cdecl *wctrans_f)(const char *);
static wctype_f p_wctype; static wctrans_f p_wctrans;

TC_CASE(wctype_unknown_returns_zero)
{
    CHECK(p_wctype != NULL); if (!p_wctype) return;
    CHECK_EQ_U(p_wctype("bogus"), 0);
    CHECK_EQ_U(p_wctype(""), 0);
    CHECK_EQ_U(p_wctype("ALPHA"), 0);   /* names are case sensitive */
    CHECK_EQ_U(p_wctype("alph"), 0);
}
TC_CASE(wctrans_unknown_returns_zero)
{
    CHECK(p_wctrans != NULL); if (!p_wctrans) return;
    CHECK_EQ_U(p_wctrans("bogus"), 0);
    CHECK_EQ_U(p_wctrans(""), 0);
    CHECK_EQ_U(p_wctrans("TOUPPER"), 0);
}
int main(void)
{
    tc_begin();
    p_wctype = (wctype_f)tc_crt_proc("wctype"); p_wctrans = (wctrans_f)tc_crt_proc("wctrans");
    TC_RUN(wctype_unknown_returns_zero); TC_RUN(wctrans_unknown_returns_zero);
    return tc_end();
}

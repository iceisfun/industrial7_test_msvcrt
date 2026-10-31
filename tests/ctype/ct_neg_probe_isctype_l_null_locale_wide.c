/* TC
 * id:    ct_neg_probe_isctype_l_null_locale_wide
 * area:  ctype
 * kind:  negative
 * funcs:
 * procs: _towlower_l _iswctype_l wctype
 * crt:   >=100
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _towlower_l, _iswctype_l
 * note:  PROBES that a NULL _locale_t means the current locale (no crash, no invalid-parameter call) for the wide
 *        twins; wide twin of ct_neg_l_variants_null_locale.
 */
#include "tc.h"

TC_DECL_PROC(unsigned short, _towlower_l, (unsigned short, void *));
TC_DECL_PROC(int, _iswctype_l, (unsigned short, unsigned short, void *));
TC_DECL_PROC(unsigned short, wctype, (const char *));
static volatile unsigned short vc;
static const char *volatile nm = "alpha";

TC_CASE(towlower_l_null)
{
    if (!tc_p__towlower_l) TC_SKIP("missing");
    vc = L'A'; CHECK_EQ_U(tc_p__towlower_l(vc, NULL), L'a');
}
TC_CASE(iswctype_l_null)
{
    unsigned short m;
    if (!tc_p__iswctype_l || !tc_p_wctype) TC_SKIP("missing");
    m = tc_p_wctype(nm);
    vc = L'a'; CHECK(tc_p__iswctype_l(vc, m, NULL) != 0);
    vc = L'5'; CHECK_EQ_I(tc_p__iswctype_l(vc, m, NULL), 0);
}
TC_CASE(no_iph)
{
    unsigned short m = tc_p_wctype ? tc_p_wctype(nm) : 0;
    tc_iph_count = 0; vc = L'A';
    if (tc_p__towlower_l) (void)tc_p__towlower_l(vc, NULL);
    if (tc_p__iswctype_l) (void)tc_p__iswctype_l(vc, m, NULL);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_towlower_l); TC_LOAD_PROC(_iswctype_l); TC_LOAD_PROC(wctype);
    tc_iph_install();
    TC_RUN(towlower_l_null); TC_RUN(iswctype_l_null); TC_RUN(no_iph);
    return tc_end();
}

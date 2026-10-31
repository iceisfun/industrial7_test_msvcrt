/* TC
 * id:    gct_neg_wide_csym_high_values
 * area:  ctype
 * funcs: 
 * procs: __iswcsym __iswcsymf _iswpunct_l _iswxdigit_l _create_locale _free_locale
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs iswxdigit, iswpunct, __iswcsym: WEOF (0xFFFF) is classified as nothing
 * note:  Only WEOF and '_' are asserted. Noncharacters, fullwidth forms and _iswcsym_l are in gct_neg_wide_csym_probe_values (probe).
 */
#include "tc.h"
typedef void *loc_t;
TC_DECL_PROC(loc_t, _create_locale, (int, const char *));
TC_DECL_PROC(void, _free_locale, (loc_t));
TC_DECL_PROC(int, __iswcsym, (unsigned short));
TC_DECL_PROC(int, __iswcsymf, (unsigned short));
TC_DECL_PROC(int, _iswpunct_l, (unsigned short, loc_t));
TC_DECL_PROC(int, _iswxdigit_l, (unsigned short, loc_t));
static loc_t loc;
static volatile unsigned short vc;

TC_CASE(ready) { CHECK(loc && tc_p___iswcsym && tc_p___iswcsymf && tc_p__iswpunct_l && tc_p__iswxdigit_l); }
TC_CASE(weof_all_zero)
{
    if (!loc || !tc_p___iswcsym || !tc_p__iswpunct_l || !tc_p__iswxdigit_l || !tc_p___iswcsymf) TC_SKIP("unavailable");
    tc_iph_install(); tc_iph_count = 0; vc = 0xFFFF;
    CHECK_EQ_I(tc_p___iswcsym(vc), 0); CHECK_EQ_I(tc_p___iswcsymf(vc), 0);
    CHECK_EQ_I(tc_p__iswpunct_l(vc, loc), 0); CHECK_EQ_I(tc_p__iswxdigit_l(vc, loc), 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(underscore)
{
    if (!loc || !tc_p___iswcsym || !tc_p__iswpunct_l || !tc_p__iswxdigit_l) TC_SKIP("unavailable");
    CHECK(tc_p___iswcsym('_')); CHECK(tc_p__iswpunct_l('_', loc)); CHECK_EQ_I(tc_p__iswxdigit_l('_', loc), 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_create_locale); TC_LOAD_PROC(_free_locale); TC_LOAD_PROC(__iswcsym); TC_LOAD_PROC(__iswcsymf);
    TC_LOAD_PROC(_iswpunct_l); TC_LOAD_PROC(_iswxdigit_l);
    if (tc_p__create_locale) loc = tc_p__create_locale(0, "C");
    TC_RUN(ready); TC_RUN(weof_all_zero); TC_RUN(underscore);
    if (loc && tc_p__free_locale) tc_p__free_locale(loc);
    return tc_end();
}

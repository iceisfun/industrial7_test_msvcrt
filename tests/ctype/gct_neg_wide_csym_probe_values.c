/* TC
 * id:    gct_neg_wide_csym_probe_values
 * area:  ctype
 * funcs:
 * procs: __iswcsym __iswcsymf _iswcsym_l _iswcsymf_l _iswxdigit_l _create_locale _free_locale
 * crt:   >=100
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs iswxdigit (ASCII hex digits only documented)
 * note:  PROBE, three guesses each in its own case: (1) U+FFFE (noncharacter) is not csym/csymf/xdigit; (2) U+FFFD is not
 *        csym/csymf (it is a symbol, not alnum); (3) fullwidth A/a/0 (U+FF21/FF41/FF10) are not xdigit. Windows may classify
 *        fullwidth forms via GetStringTypeW C1_XDIGIT, in which case (3) will show it. _iswcsym_l export in msvcr100-120 unverified.
 *        Observed Win10 22H2 ucrtbase: (3) is TRUE, _iswxdigit_l(U+FF21/FF41/FF10) returns 128 (_HEX); deviates from C11 7.30.2.1.12
 *        (only 0-9a-fA-F). Cases (1)(2) pass as originally guessed. Stable.
 */
#include "tc.h"
typedef void *loc_t;
TC_DECL_PROC(loc_t, _create_locale, (int, const char *));
TC_DECL_PROC(void, _free_locale, (loc_t));
TC_DECL_PROC(int, __iswcsym, (unsigned short));
TC_DECL_PROC(int, __iswcsymf, (unsigned short));
TC_DECL_PROC(int, _iswcsym_l, (unsigned short, loc_t));
TC_DECL_PROC(int, _iswcsymf_l, (unsigned short, loc_t));
TC_DECL_PROC(int, _iswxdigit_l, (unsigned short, loc_t));
static loc_t loc;
static volatile unsigned short vc;

TC_CASE(fffe_not_classified)
{
    if (!loc || !tc_p___iswcsym || !tc_p___iswcsymf || !tc_p__iswxdigit_l) TC_SKIP("unavailable");
    vc = 0xFFFE;
    CHECK_EQ_I(tc_p___iswcsym(vc), 0); CHECK_EQ_I(tc_p___iswcsymf(vc), 0); CHECK_EQ_I(tc_p__iswxdigit_l(vc, loc), 0);
}
TC_CASE(fffd_not_csym)
{
    if (!tc_p___iswcsym || !tc_p___iswcsymf) TC_SKIP("unavailable");
    vc = 0xFFFD; CHECK_EQ_I(tc_p___iswcsym(vc), 0); CHECK_EQ_I(tc_p___iswcsymf(vc), 0);
}
TC_CASE(weof_l_forms)
{
    if (!loc || !tc_p__iswcsym_l || !tc_p__iswcsymf_l) TC_SKIP("unavailable");
    vc = 0xFFFF; CHECK_EQ_I(tc_p__iswcsym_l(vc, loc), 0); CHECK_EQ_I(tc_p__iswcsymf_l(vc, loc), 0);
}
TC_CASE(fullwidth_not_xdigit)  /* name kept; observed: fullwidth forms ARE xdigit */
{
    if (!loc || !tc_p__iswxdigit_l) TC_SKIP("unavailable");
    vc = 0xFF21; CHECK(tc_p__iswxdigit_l(vc, loc) != 0);   /* observed: 128 */
    vc = 0xFF41; CHECK(tc_p__iswxdigit_l(vc, loc) != 0);
    vc = 0xFF10; CHECK(tc_p__iswxdigit_l(vc, loc) != 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_create_locale); TC_LOAD_PROC(_free_locale); TC_LOAD_PROC(__iswcsym); TC_LOAD_PROC(__iswcsymf);
    TC_LOAD_PROC(_iswcsym_l); TC_LOAD_PROC(_iswcsymf_l); TC_LOAD_PROC(_iswxdigit_l);
    if (tc_p__create_locale) loc = tc_p__create_locale(0, "C");
    TC_RUN(fffe_not_classified); TC_RUN(fffd_not_csym); TC_RUN(weof_l_forms); TC_RUN(fullwidth_not_xdigit);
    if (loc && tc_p__free_locale) tc_p__free_locale(loc);
    return tc_end();
}

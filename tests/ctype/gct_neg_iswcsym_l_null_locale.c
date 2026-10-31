/* TC
 * id:    gct_neg_iswcsym_l_null_locale
 * area:  ctype
 * funcs:
 * procs: _iswcsym_l _iswcsymf_l _iswpunct_l _iswxdigit_l
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs isalpha, _isalpha_l, _iswalpha_l ("locale NULL" uses current locale in UCRT)
 * note:  PROBE: with a NULL _locale_t the UCRT _isw*_l functions are believed to fall back to the current locale
 *        (no invalid-parameter call). Asserts nonzero result and tc_iph_count==0 for the four functions.
 */
#include "tc.h"

typedef int (__cdecl *isw_l_t)(unsigned int, void *);
static isw_l_t p_xdigit, p_punct, p_csym, p_csymf;

static void probe(isw_l_t f, unsigned int c, int *r)
{
    int before = tc_iph_count;
    *r = f(c, NULL);
    CHECK_EQ_I(tc_iph_count, before);
}
TC_CASE(exported) { CHECK(p_xdigit && p_punct && p_csym && p_csymf); }
TC_CASE(xdigit_null) { int r = 0; if (!p_xdigit) TC_SKIP("missing"); probe(p_xdigit, 'A', &r); CHECK(r != 0); }
TC_CASE(punct_null)  { int r = 0; if (!p_punct)  TC_SKIP("missing"); probe(p_punct,  '!', &r); CHECK(r != 0); }
TC_CASE(csym_null)   { int r = 0; if (!p_csym)   TC_SKIP("missing"); probe(p_csym,   'a', &r); CHECK(r != 0); }
TC_CASE(csymf_null)  { int r = 0; if (!p_csymf)  TC_SKIP("missing"); probe(p_csymf,  'a', &r); CHECK(r != 0); }

int main(void)
{
    tc_begin();
    p_xdigit = (isw_l_t)tc_crt_proc("_iswxdigit_l");
    p_punct  = (isw_l_t)tc_crt_proc("_iswpunct_l");
    p_csym   = (isw_l_t)tc_crt_proc("_iswcsym_l");
    p_csymf  = (isw_l_t)tc_crt_proc("_iswcsymf_l");
    if (!tc_iph_install()) return 2;
    TC_RUN(exported); TC_RUN(xdigit_null); TC_RUN(punct_null); TC_RUN(csym_null); TC_RUN(csymf_null);
    return tc_end();
}

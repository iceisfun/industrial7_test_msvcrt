/* TC
 * id:    gwc_iswclass_l_ucrt_unicode_probe
 * area:  conv
 * funcs:
 * procs: _iswalnum_l _iswcntrl_l _iswgraph_l _iswprint_l _create_locale _free_locale
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs iswalnum; C11 7.30.2.1
 * note:  PROBE: classification of wide chars above 0x7F through a "C" locale object in UCRT. Believed to follow Unicode tables
 *        for CJK (U+4E00 alnum/graph/print) and C1 controls (U+0085 cntrl); U+00E9 alnum, U+00A0 print and U+2028 cntrl
 *        are weaker guesses. Every char/function is its own case so one wrong guess does not mask others.
 *        OBSERVED (Win10 22H2 19045 ucrtbase, run twice): the two weak guesses were wrong. _iswcntrl_l(0x2028, C-locale) == 0 and
 *        _iswprint_l(0x00A0, C-locale) == 0; the other six cases (CJK alnum/graph/print, U+00E9 alnum, U+0085 cntrl and not print) held.
 *        Cases renamed u2028_not_cntrl / nbsp_not_print. Table-driven: stable across runs; depends on the ucrtbase build's ctype tables.
 */
#include "tc.h"

typedef int (__cdecl *isw_l_t)(unsigned int, void *);
TC_DECL_PROC(void *, _create_locale, (int, const char *));
TC_DECL_PROC(void, _free_locale, (void *));
static isw_l_t f_alnum, f_cntrl, f_graph, f_print;
static void *loc;

static int load(void)
{
    TC_LOAD_PROC(_create_locale); TC_LOAD_PROC(_free_locale);
    f_alnum = (isw_l_t)tc_crt_proc("_iswalnum_l"); f_cntrl = (isw_l_t)tc_crt_proc("_iswcntrl_l");
    f_graph = (isw_l_t)tc_crt_proc("_iswgraph_l"); f_print = (isw_l_t)tc_crt_proc("_iswprint_l");
    return f_alnum && f_cntrl && f_graph && f_print;
}

TC_CASE(cjk_alnum) { if (!f_alnum || !loc) TC_SKIP("missing"); CHECK_EQ_I(f_alnum(0x4E00, loc) != 0, 1); }
TC_CASE(cjk_graph) { if (!f_graph || !loc) TC_SKIP("missing"); CHECK_EQ_I(f_graph(0x4E00, loc) != 0, 1); }
TC_CASE(cjk_print) { if (!f_print || !loc) TC_SKIP("missing"); CHECK_EQ_I(f_print(0x4E00, loc) != 0, 1); }
TC_CASE(eacute_alnum) { if (!f_alnum || !loc) TC_SKIP("missing"); CHECK_EQ_I(f_alnum(0x00E9, loc) != 0, 1); }
TC_CASE(c1_0085_cntrl) { if (!f_cntrl || !loc) TC_SKIP("missing"); CHECK_EQ_I(f_cntrl(0x0085, loc) != 0, 1); }
TC_CASE(c1_0085_print_false) { if (!f_print || !loc) TC_SKIP("missing"); CHECK_EQ_I(f_print(0x0085, loc) != 0, 0); }
TC_CASE(u2028_not_cntrl) { if (!f_cntrl || !loc) TC_SKIP("missing"); CHECK_EQ_I(f_cntrl(0x2028, loc) != 0, 0); }   /* observed: U+2028 is not iswcntrl */
TC_CASE(nbsp_not_print) { if (!f_print || !loc) TC_SKIP("missing"); CHECK_EQ_I(f_print(0x00A0, loc) != 0, 0); }   /* observed: U+00A0 is not iswprint in the C locale object */

int main(void)
{
    tc_begin();
    if (!load() || !tc_p__create_locale) { tc_write("TC SKIP gwc_iswclass_l_ucrt_unicode_probe::all missing exports\n"); return tc_end(); }
    loc = tc_p__create_locale(0, "C");
    CHECK(loc != NULL);
    TC_RUN(cjk_alnum); TC_RUN(cjk_graph); TC_RUN(cjk_print); TC_RUN(eacute_alnum);
    TC_RUN(c1_0085_cntrl); TC_RUN(c1_0085_print_false); TC_RUN(u2028_not_cntrl); TC_RUN(nbsp_not_print);
    if (loc && tc_p__free_locale) tc_p__free_locale(loc);
    return tc_end();
}

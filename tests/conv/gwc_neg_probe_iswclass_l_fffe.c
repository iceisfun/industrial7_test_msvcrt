/* TC
 * id:    gwc_neg_probe_iswclass_l_fffe
 * area:  conv
 * funcs:
 * procs: _iswalnum_l _iswcntrl_l _iswgraph_l _iswprint_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.30.1; MS docs iswalnum
 * note:  PROBE: U+FFFE (a Unicode noncharacter, not WEOF) is believed to be classified as none of alnum/cntrl/graph/print through
 *        a "C" locale object. Each function in its own case.
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

static volatile wint_t v;

TC_CASE(fffe_alnum) { if (!f_alnum || !loc) TC_SKIP("missing"); v = 0xFFFE; CHECK_EQ_I(f_alnum(v, loc) != 0, 0); }
TC_CASE(fffe_cntrl) { if (!f_cntrl || !loc) TC_SKIP("missing"); v = 0xFFFE; CHECK_EQ_I(f_cntrl(v, loc) != 0, 0); }
TC_CASE(fffe_graph) { if (!f_graph || !loc) TC_SKIP("missing"); v = 0xFFFE; CHECK_EQ_I(f_graph(v, loc) != 0, 0); }
TC_CASE(fffe_print) { if (!f_print || !loc) TC_SKIP("missing"); v = 0xFFFE; CHECK_EQ_I(f_print(v, loc) != 0, 0); }

int main(void)
{
    tc_begin();
    if (!load() || !tc_p__create_locale) { tc_write("TC SKIP gwc_neg_probe_iswclass_l_fffe::all missing exports\n"); return tc_end(); }
    loc = tc_p__create_locale(0, "C");
    CHECK(loc != NULL);
    TC_RUN(fffe_alnum); TC_RUN(fffe_cntrl); TC_RUN(fffe_graph); TC_RUN(fffe_print);
    if (loc && tc_p__free_locale) tc_p__free_locale(loc);
    return tc_end();
}

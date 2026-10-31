/* TC
 * id:    gwc_neg_iswclass_l_null_locale_probe
 * area:  conv
 * funcs:
 * procs: _iswalnum_l _iswcntrl_l _iswgraph_l _iswprint_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _isalpha_l / _iswalpha_l (locale parameter); see ct_neg_probe_isctype_l_null_locale_wide
 * note:  PROBE: a NULL _locale_t passed to _iswalnum_l/_iswcntrl_l/_iswgraph_l/_iswprint_l is believed to mean the current
 *        locale (nonzero result for 'A', '\t', '!', ' ' respectively, no invalid-parameter call). Undocumented; each in its own case.
 *        tc_iph_count is also checked: a handler call would be recorded as a failure of the "no iph" assertion.
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

TC_CASE(alnum_null) { int r, before = tc_iph_count; if (!f_alnum) TC_SKIP("missing"); r = f_alnum(L'A', NULL); CHECK(r != 0); CHECK_EQ_I(tc_iph_count, before); }
TC_CASE(cntrl_null) { int r, before = tc_iph_count; if (!f_cntrl) TC_SKIP("missing"); r = f_cntrl(L'\t', NULL); CHECK(r != 0); CHECK_EQ_I(tc_iph_count, before); }
TC_CASE(graph_null) { int r, before = tc_iph_count; if (!f_graph) TC_SKIP("missing"); r = f_graph(L'!', NULL); CHECK(r != 0); CHECK_EQ_I(tc_iph_count, before); }
TC_CASE(print_null) { int r, before = tc_iph_count; if (!f_print) TC_SKIP("missing"); r = f_print(L' ', NULL); CHECK(r != 0); CHECK_EQ_I(tc_iph_count, before); }

int main(void)
{
    tc_begin();
    if (!load()) { tc_write("TC SKIP gwc_neg_iswclass_l_null_locale_probe::all missing exports\n"); return tc_end(); }
    tc_iph_install();
    TC_RUN(alnum_null); TC_RUN(cntrl_null); TC_RUN(graph_null); TC_RUN(print_null);
    return tc_end();
}

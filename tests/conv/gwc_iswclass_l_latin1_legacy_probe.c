/* TC
 * id:    gwc_iswclass_l_latin1_legacy_probe
 * area:  conv
 * funcs:
 * procs: _iswalnum_l _iswcntrl_l _iswgraph_l _iswprint_l _create_locale _free_locale
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _iswalnum_l, iswalpha
 * note:  PROBE: legacy CRTs classify U+00E9 / U+4E00 as alnum, U+0080 as cntrl and U+00A0 as printable even through a "C"
 *        locale object (wide classification believed to use the Unicode ctype table regardless of locale). Undocumented.
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

TC_CASE(eacute_alnum) { if (!f_alnum || !loc) TC_SKIP("missing"); CHECK_EQ_I(f_alnum(0x00E9, loc) != 0, 1); }
TC_CASE(cjk_alnum) { if (!f_alnum || !loc) TC_SKIP("missing"); CHECK_EQ_I(f_alnum(0x4E00, loc) != 0, 1); }
TC_CASE(c1_0080_cntrl) { if (!f_cntrl || !loc) TC_SKIP("missing"); CHECK_EQ_I(f_cntrl(0x0080, loc) != 0, 1); }
TC_CASE(nbsp_print) { if (!f_print || !loc) TC_SKIP("missing"); CHECK_EQ_I(f_print(0x00A0, loc) != 0, 1); }

int main(void)
{
    tc_begin();
    if (!load() || !tc_p__create_locale) { tc_write("TC SKIP gwc_iswclass_l_latin1_legacy_probe::all missing exports\n"); return tc_end(); }
    loc = tc_p__create_locale(0, "C");
    CHECK(loc != NULL);
    TC_RUN(eacute_alnum); TC_RUN(cjk_alnum); TC_RUN(c1_0080_cntrl); TC_RUN(nbsp_print);
    if (loc && tc_p__free_locale) tc_p__free_locale(loc);
    return tc_end();
}

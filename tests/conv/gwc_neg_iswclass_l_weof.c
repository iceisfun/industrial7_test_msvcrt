/* TC
 * id:    gwc_neg_iswclass_l_weof
 * area:  conv
 * funcs: is_wctype
 * procs: _iswalnum_l _iswcntrl_l _iswgraph_l _iswprint_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.30.1 (WEOF is not a wide character; result for it is not specified by the standard); MS docs iswalnum
 * note:  WEOF is 0xFFFF. The CRT special-cases WEOF in its wide classification and returns 0 (known real-Windows behaviour, not
 *        a documented guarantee). U+FFFE (noncharacter) is probed separately in gwc_neg_probe_iswclass_l_fffe.
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
static void setup(void) { if (tc_p__create_locale) loc = tc_p__create_locale(0, "C"); }

TC_CASE(weof_all_four_zero)
{
    int before = tc_iph_count;
    if (!f_alnum || !loc) TC_SKIP("missing");
    v = WEOF;
    CHECK_EQ_I(f_alnum(v, loc), 0); CHECK_EQ_I(f_cntrl(v, loc), 0);
    CHECK_EQ_I(f_graph(v, loc), 0); CHECK_EQ_I(f_print(v, loc), 0);
    CHECK_EQ_I(tc_iph_count, before);
}
TC_CASE(weof_is_ffff) { v = WEOF; CHECK_EQ_U(v, 0xFFFF); CHECK_EQ_U(v, (wint_t)L'\xFFFF'); }
TC_CASE(weof_is_wctype_masks)
{
    unsigned short masks[] = { _UPPER, _LOWER, _DIGIT, _SPACE, _PUNCT, _CONTROL, _HEX, _ALPHA, _ALPHA | _DIGIT };
    size_t k;
    for (k = 0; k < sizeof masks / sizeof masks[0]; k++) {
        volatile unsigned short m = masks[k];
        v = WEOF;
        CHECK_EQ_I(is_wctype(v, m), 0);
    }
}

int main(void)
{
    tc_begin();
    if (!load()) { tc_write("TC SKIP gwc_neg_iswclass_l_weof::all missing exports\n"); return tc_end(); }
    tc_iph_install(); setup();
    CHECK(loc != NULL);
    TC_RUN(weof_all_four_zero); TC_RUN(weof_is_ffff); TC_RUN(weof_is_wctype_masks);
    if (loc && tc_p__free_locale) tc_p__free_locale(loc);
    return tc_end();
}

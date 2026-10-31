/* TC
 * id:    gwc_iswclass_l_ascii
 * area:  conv
 * funcs:
 * procs: _iswalnum_l _iswcntrl_l _iswgraph_l _iswprint_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.30.2.1.1/.4/.6/.8 (iswalnum iswcntrl iswgraph iswprint, ASCII in the C locale); MS docs iswalnum, _iswalnum_l
 * note:  Results are compared with != 0. msvcrt.dll/msvcr80 omitted: _create_locale availability there is not certain.
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

TC_CASE(exported) { CHECK(f_alnum && f_cntrl && f_graph && f_print); CHECK(tc_p__create_locale != NULL); }
TC_CASE(ascii_counts)
{
    int c, na = 0, nc = 0, ng = 0, np = 0;
    if (!f_alnum || !loc) TC_SKIP("missing");
    for (c = 0; c < 128; c++) {
        int a = f_alnum(c, loc) != 0, k = f_cntrl(c, loc) != 0, g = f_graph(c, loc) != 0, p = f_print(c, loc) != 0;
        na += a; nc += k; ng += g; np += p;
        if (a != ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) tc_fail_ints(__FILE__, __LINE__, "alnum(c)", "expected", a, !a);
        if (k != (c < 32 || c == 127)) tc_fail_ints(__FILE__, __LINE__, "cntrl(c)", "expected", k, !k);
        if (g != (c > 32 && c < 127)) tc_fail_ints(__FILE__, __LINE__, "graph(c)", "expected", g, !g);
        if (p != (c >= 32 && c < 127)) tc_fail_ints(__FILE__, __LINE__, "print(c)", "expected", p, !p);
    }
    CHECK_EQ_I(na, 62); CHECK_EQ_I(nc, 33); CHECK_EQ_I(ng, 94); CHECK_EQ_I(np, 95);
}
TC_CASE(specific_points)
{
    if (!f_alnum || !loc) TC_SKIP("missing");
    CHECK(f_graph(0x21, loc) != 0); CHECK(f_graph(0x7E, loc) != 0); CHECK_EQ_I(f_graph(0x20, loc) != 0, 0);
    CHECK(f_print(0x20, loc) != 0); CHECK(f_print(0x7E, loc) != 0); CHECK_EQ_I(f_print(0x7F, loc) != 0, 0);
    CHECK(f_cntrl(0x7F, loc) != 0); CHECK(f_cntrl(0, loc) != 0); CHECK_EQ_I(f_cntrl(0x20, loc) != 0, 0);
}
TC_CASE(relations)
{
    int c, bad = 0;
    if (!f_alnum || !loc) TC_SKIP("missing");
    for (c = 0; c < 128; c++) {
        int g = f_graph(c, loc) != 0, p = f_print(c, loc) != 0, a = f_alnum(c, loc) != 0, k = f_cntrl(c, loc) != 0;
        if (g != (p && c != ' ')) bad++;
        if (a && !g) bad++;
        if (k && p) bad++;
    }
    CHECK_EQ_I(bad, 0);
}

int main(void)
{
    tc_begin();
    load();
    if (tc_p__create_locale) loc = tc_p__create_locale(0, "C");
    CHECK(loc != NULL || !tc_p__create_locale);
    TC_RUN(exported); TC_RUN(ascii_counts); TC_RUN(specific_points); TC_RUN(relations);
    if (loc && tc_p__free_locale) tc_p__free_locale(loc);
    return tc_end();
}

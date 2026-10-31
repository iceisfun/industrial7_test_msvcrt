/* TC
 * id:    gct_iswcsym_l_forms_probe
 * area:  ctype
 * funcs:
 * procs: _iswcsym_l _iswcsymf_l __iswcsym __iswcsymf _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs __iswcsym, _iswcsym_l, _iswcsymf_l
 * note:  PROBE: whether _iswcsym_l/_iswcsymf_l are real exports in msvcr90..msvcr120 (they are macros in some CRT headers),
 *        and that for ASCII they agree with the non-_l forms and classify as [A-Za-z0-9_] / [A-Za-z_] in a "C" locale object.
 */
#include "tc.h"
typedef void *loc_t;
TC_DECL_PROC(loc_t, _create_locale, (int, const char *));
TC_DECL_PROC(void, _free_locale, (loc_t));
TC_DECL_PROC(int, __iswcsym, (unsigned short));
TC_DECL_PROC(int, __iswcsymf, (unsigned short));
TC_DECL_PROC(int, _iswcsym_l, (unsigned short, loc_t));
TC_DECL_PROC(int, _iswcsymf_l, (unsigned short, loc_t));
static loc_t loc;
static int alpha(int c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
static int digit(int c) { return c >= '0' && c <= '9'; }

TC_CASE(loaded) { CHECK(tc_p__iswcsym_l && tc_p__iswcsymf_l); CHECK(loc != NULL); }
TC_CASE(l_forms_ascii)
{
    int c; if (!loc || !tc_p__iswcsym_l || !tc_p__iswcsymf_l) TC_SKIP("unavailable");
    for (c = 0; c < 128; c++) {
        if ((tc_p__iswcsym_l((unsigned short)c, loc) != 0) != (alpha(c) || digit(c) || c == '_')) { CHECK_EQ_I(c, -1); }
        if ((tc_p__iswcsymf_l((unsigned short)c, loc) != 0) != (alpha(c) || c == '_')) { CHECK_EQ_I(c, -2); }
    }
}
TC_CASE(l_forms_agree)
{
    int c; if (!loc || !tc_p__iswcsym_l || !tc_p__iswcsymf_l || !tc_p___iswcsym || !tc_p___iswcsymf) TC_SKIP("unavailable");
    for (c = 0; c < 128; c++) {
        if ((tc_p__iswcsym_l((unsigned short)c, loc) != 0) != (tc_p___iswcsym((unsigned short)c) != 0)) { CHECK_EQ_I(c, -1); }
        if ((tc_p__iswcsymf_l((unsigned short)c, loc) != 0) != (tc_p___iswcsymf((unsigned short)c) != 0)) { CHECK_EQ_I(c, -2); }
    }
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_create_locale); TC_LOAD_PROC(_free_locale); TC_LOAD_PROC(__iswcsym); TC_LOAD_PROC(__iswcsymf);
    TC_LOAD_PROC(_iswcsym_l); TC_LOAD_PROC(_iswcsymf_l);
    if (tc_p__create_locale) loc = tc_p__create_locale(0, "C");
    TC_RUN(loaded); TC_RUN(l_forms_ascii); TC_RUN(l_forms_agree);
    if (loc && tc_p__free_locale) tc_p__free_locale(loc);
    return tc_end();
}

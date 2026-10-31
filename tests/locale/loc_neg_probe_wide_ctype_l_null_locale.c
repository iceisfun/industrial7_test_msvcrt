/* TC
 * id:    loc_neg_probe_wide_ctype_l_null_locale
 * area:  locale
 * procs: _iswalpha_l _iswdigit_l
 * funcs: _towlower_l _towupper_l
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   CRT convention: NULL _locale_t means the current locale
 * note:  PROBE: tc_p__iswalpha_l(L'A', NULL) etc. with a NULL locale must not crash and behave like the current ("C") locale.
 */
#include "tc.h"
#include <wctype.h>
#include <locale.h>

TC_DECL_PROC(int, _iswalpha_l, (wint_t, _locale_t));
TC_DECL_PROC(int, _iswdigit_l, (wint_t, _locale_t));

static _locale_t volatile v_nl;
static volatile wint_t v_A = L'A', v_a = L'a', v_5 = L'5';

TC_CASE(null_locale_classifies)
{
    setlocale(LC_ALL, "C");
    CHECK(tc_p__iswalpha_l(v_A, v_nl) != 0); CHECK_EQ_I(tc_p__iswalpha_l(v_5, v_nl), 0);
    CHECK(tc_p__iswdigit_l(v_5, v_nl) != 0); CHECK_EQ_I(tc_p__iswdigit_l(v_A, v_nl), 0);
}
TC_CASE(null_locale_case_map)
{
    setlocale(LC_ALL, "C");
    CHECK_EQ_U(_towlower_l(v_A, v_nl), L'a'); CHECK_EQ_U(_towupper_l(v_a, v_nl), L'A');
}
int main(void) { tc_begin(); TC_LOAD_PROC(_iswalpha_l); TC_LOAD_PROC(_iswdigit_l);
    if (!tc_p__iswalpha_l || !tc_p__iswdigit_l) { tc_write("TC FAIL " TC_TEST_ID "::__missing proc\n"); return 1; } TC_RUN(null_locale_classifies); TC_RUN(null_locale_case_map); return tc_end(); }

/* TC
 * id:    loc_neg_wide_ctype_l_null_and_range
 * area:  locale
 * procs: _iswalpha_l _iswdigit_l
 * funcs: _towlower_l _towupper_l _iswctype_l iswctype
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   C11 7.30.2 (WEOF is not a wide character: iswalpha etc. false); MS docs iswalpha/towlower/iswctype
 * note:  Boundary values WEOF (0xFFFF), 0 and mask 0 must not crash and classify as nothing. towlower/towupper map WEOF to itself.
 *        The NULL-locale behaviour is probed separately in loc_neg_probe_wide_ctype_l_null_locale.
 */
#include "tc.h"
#include <wctype.h>
#include <locale.h>

TC_DECL_PROC(int, _iswalpha_l, (wint_t, _locale_t));
TC_DECL_PROC(int, _iswdigit_l, (wint_t, _locale_t));

static volatile wint_t v_weof = WEOF, v_ffff = 0xFFFF, v_zero = 0, v_A = L'A';
static volatile unsigned short v_mask0 = 0;

TC_CASE(weof_not_classified)
{
    _locale_t l = _create_locale(LC_ALL, "C"); if (!l) TC_SKIP("no locale");
    CHECK_EQ_I(tc_p__iswalpha_l(v_weof, l), 0); CHECK_EQ_I(tc_p__iswdigit_l(v_weof, l), 0);
    CHECK_EQ_I(tc_p__iswalpha_l(v_ffff, l), 0); CHECK_EQ_I(tc_p__iswdigit_l(v_ffff, l), 0);
    _free_locale(l);
}
TC_CASE(zero_not_classified)
{
    _locale_t l = _create_locale(LC_ALL, "C"); if (!l) TC_SKIP("no locale");
    CHECK_EQ_I(tc_p__iswalpha_l(v_zero, l), 0); CHECK_EQ_I(tc_p__iswdigit_l(v_zero, l), 0);
    _free_locale(l);
}
TC_CASE(case_map_weof)
{
    _locale_t l = _create_locale(LC_ALL, "C"); if (!l) TC_SKIP("no locale");
    CHECK_EQ_U(_towlower_l(v_weof, l), WEOF); CHECK_EQ_U(_towupper_l(v_ffff, l), 0xFFFF);
    CHECK_EQ_U(_towupper_l(v_weof, l), WEOF);
    _free_locale(l);
}
TC_CASE(iswctype_mask_zero)
{
    _locale_t l = _create_locale(LC_ALL, "C"); if (!l) TC_SKIP("no locale");
    CHECK_EQ_I(iswctype(v_A, v_mask0), 0);
    CHECK_EQ_I(_iswctype_l(v_A, v_mask0, l), 0);
    CHECK_EQ_I(iswctype(v_weof, _ALPHA), 0);
    _free_locale(l);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_iswalpha_l); TC_LOAD_PROC(_iswdigit_l);
    if (!tc_p__iswalpha_l || !tc_p__iswdigit_l) { tc_write("TC FAIL " TC_TEST_ID "::__missing proc\n"); return 1; }
    TC_RUN(weof_not_classified); TC_RUN(zero_not_classified); TC_RUN(case_map_weof); TC_RUN(iswctype_mask_zero);
    return tc_end();
}

/* TC
 * id:    gst_wcstold_l_basic
 * area:  conv
 * funcs: _create_locale _free_locale
 * procs: _wcstold_l
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strtold, wcstold, _wcstold_l (long double is the same as double on MSVC)
 * note:  mingw long double is 80-bit, so the function is called through a proc typed as returning double.
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <float.h>

TC_DECL_PROC(double, _wcstold_l, (const wchar_t *, wchar_t **, _locale_t));
static _locale_t loc;
static const wchar_t *volatile v;

TC_CASE(simple) { v = L"1.5"; CHECK_BITS_D(tc_p__wcstold_l(v, NULL, loc), 1.5); v = L"-0.0"; CHECK_BITS_D(tc_p__wcstold_l(v, NULL, loc), -0.0); }
TC_CASE(limits)
{
    v = L"1e308"; CHECK_BITS_D(tc_p__wcstold_l(v, NULL, loc), 1e308);
    v = L"1.7976931348623157e308"; CHECK_BITS_D(tc_p__wcstold_l(v, NULL, loc), DBL_MAX);
    v = L"4.9406564584124654e-324"; CHECK_EQ_U(tc_dbits(tc_p__wcstold_l(v, NULL, loc)), 1u);
}
TC_CASE(endptr_whitespace)
{
    wchar_t *e = NULL;
    v = L"2.5xyz"; CHECK_BITS_D(tc_p__wcstold_l(v, &e, loc), 2.5); CHECK_EQ_P(e, v + 3);
    v = L"\t  -8";   e = NULL; CHECK_BITS_D(tc_p__wcstold_l(v, &e, loc), -8.0); CHECK_EQ_P(e, v + 5);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_wcstold_l);
    if (!tc_p__wcstold_l) { tc_write("TC FAIL " TC_TEST_ID "::__wcstold_l not exported\n"); return 1; }
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(simple); TC_RUN(limits); TC_RUN(endptr_whitespace);
    _free_locale(loc);
    return tc_end();
}

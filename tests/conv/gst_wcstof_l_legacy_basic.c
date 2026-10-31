/* TC
 * id:    gst_wcstof_l_legacy_basic
 * area:  conv
 * funcs: _create_locale _free_locale
 * procs: _wcstof_l
 * crt:   msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs wcstof, _wcstof_l; C11 7.29.4.1.1
 * note:  Probe: assumes that IF a pre-msvcr120 CRT exports _wcstof_l it returns float with C-standard parsing.
 *        Whether pre-msvcr120 CRTs export _wcstof_l at all is not documented (see gst_strtof_l_neg_legacy_export_probe); the test skips when the export is absent.
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <float.h>

TC_DECL_PROC(float, _wcstof_l, (const wchar_t *, wchar_t **, _locale_t));
static _locale_t loc;
static const wchar_t *volatile v;

TC_CASE(simple) { v = L"1.5"; CHECK_EQ_U(tc_fbits(tc_p__wcstof_l(v, NULL, loc)), 0x3fc00000u); v = L"-0.25"; CHECK_BITS_F(tc_p__wcstof_l(v, NULL, loc), -0.25f); }
TC_CASE(endptr) { wchar_t *e = NULL; v = L"1e2x"; CHECK_BITS_F(tc_p__wcstof_l(v, &e, loc), 100.0f); CHECK_EQ_P(e, v + 3); }
TC_CASE(flt_max) { v = L"3.4028234663852886e38"; CHECK_BITS_F(tc_p__wcstof_l(v, NULL, loc), FLT_MAX); }
TC_CASE(whitespace) { wchar_t *e = NULL; v = L" \t 6.5"; CHECK_BITS_F(tc_p__wcstof_l(v, &e, loc), 6.5f); CHECK_EQ_P(e, v + 6); }
TC_CASE(no_digits) { wchar_t *e = (wchar_t *)1; v = L"xyz"; CHECK_EQ_U(tc_fbits(tc_p__wcstof_l(v, &e, loc)), 0u); CHECK_EQ_P(e, v); }
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_wcstof_l);
    if (!tc_p__wcstof_l) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID " _wcstof_l not exported by this CRT\n"); return tc_end(); }
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(simple); TC_RUN(endptr); TC_RUN(flt_max); TC_RUN(whitespace); TC_RUN(no_digits);
    _free_locale(loc);
    return tc_end();
}

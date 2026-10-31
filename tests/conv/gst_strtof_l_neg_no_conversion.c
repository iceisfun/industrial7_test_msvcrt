/* TC
 * id:    gst_strtof_l_neg_no_conversion
 * area:  conv
 * funcs: _create_locale _free_locale
 * procs: _strtof_l
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.3p8 (no conversion: zero, endptr = nptr); MS docs strtof
 * note:  _strtof_l is not available in msvcr110 and older import sets here (see gst_strtof_l_neg_legacy_export_probe). errno after no-conversion is deliberately not asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

TC_DECL_PROC(float, _strtof_l, (const char *, char **, _locale_t));
static _locale_t loc;
static const char *volatile v;

TC_CASE(no_digits)
{
    char *e;
    v = "abc"; e = (char *)1; CHECK_EQ_U(tc_fbits(tc_p__strtof_l(v, &e, loc)), 0u); CHECK_EQ_P(e, v);
    v = "";    e = (char *)1; CHECK_EQ_U(tc_fbits(tc_p__strtof_l(v, &e, loc)), 0u); CHECK_EQ_P(e, v);
    v = "   "; e = (char *)1; CHECK_EQ_U(tc_fbits(tc_p__strtof_l(v, &e, loc)), 0u); CHECK_EQ_P(e, v);
}
TC_CASE(dot_and_exp_only)
{
    char *e;
    v = ".";   e = (char *)1; CHECK_EQ_U(tc_fbits(tc_p__strtof_l(v, &e, loc)), 0u); CHECK_EQ_P(e, v);
    v = "e5";  e = (char *)1; CHECK_EQ_U(tc_fbits(tc_p__strtof_l(v, &e, loc)), 0u); CHECK_EQ_P(e, v);
    v = "+";   e = (char *)1; CHECK_EQ_U(tc_fbits(tc_p__strtof_l(v, &e, loc)), 0u); CHECK_EQ_P(e, v);
}
TC_CASE(bare_exponent)
{
    char *e = NULL;
    v = "1e";  CHECK_BITS_F(tc_p__strtof_l(v, &e, loc), 1.0f); CHECK_EQ_P(e, v + 1);
    v = "2e+"; e = NULL; CHECK_BITS_F(tc_p__strtof_l(v, &e, loc), 2.0f); CHECK_EQ_P(e, v + 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_strtof_l);
    if (!tc_p__strtof_l) { tc_write("TC FAIL " TC_TEST_ID "::__strtof_l not exported\n"); return 1; }
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(no_digits); TC_RUN(dot_and_exp_only); TC_RUN(bare_exponent);
    _free_locale(loc);
    return tc_end();
}

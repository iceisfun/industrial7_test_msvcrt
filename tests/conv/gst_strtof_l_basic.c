/* TC
 * id:    gst_strtof_l_basic
 * area:  conv
 * funcs: _create_locale _free_locale
 * procs: _strtof_l
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.22.1.3; MS docs strtof, _strtof_l, strtof_l
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <float.h>

TC_DECL_PROC(float, _strtof_l, (const char *, char **, _locale_t));
static _locale_t loc;
static const char *volatile v;

TC_CASE(simple)
{
    v = "1.5";   CHECK_BITS_F(tc_p__strtof_l(v, NULL, loc), 1.5f); CHECK_EQ_U(tc_fbits(tc_p__strtof_l(v, NULL, loc)), 0x3fc00000u);
    v = "-0.0";  CHECK_EQ_U(tc_fbits(tc_p__strtof_l(v, NULL, loc)), 0x80000000u);
    v = "0.25";  CHECK_BITS_F(tc_p__strtof_l(v, NULL, loc), 0.25f);
    v = "0.125"; CHECK_BITS_F(tc_p__strtof_l(v, NULL, loc), 0.125f);
}
TC_CASE(nearest) { v = "3.14159274"; CHECK_EQ_U(tc_fbits(tc_p__strtof_l(v, NULL, loc)), 0x40490fdbu); }
TC_CASE(exponent)
{
    v = "1e3";  CHECK_NEAR(tc_p__strtof_l(v, NULL, loc), 1000.0, 1e-3);
    v = "1E-2"; CHECK_NEAR(tc_p__strtof_l(v, NULL, loc), 0.01, 1e-8);
}
TC_CASE(limits)
{
    v = "3.4028234663852886e38"; CHECK_BITS_F(tc_p__strtof_l(v, NULL, loc), FLT_MAX);
    v = "1.17549435e-38";       CHECK_BITS_F(tc_p__strtof_l(v, NULL, loc), FLT_MIN);
}
TC_CASE(endptr)
{
    char *e = NULL;
    v = "12.5abc"; CHECK_BITS_F(tc_p__strtof_l(v, &e, loc), 12.5f); CHECK_EQ_P(e, v + 4);
    v = "7";       e = NULL; CHECK_BITS_F(tc_p__strtof_l(v, &e, loc), 7.0f); CHECK_EQ_P(e, v + 1);
}
TC_CASE(whitespace_sign)
{
    char *e = NULL;
    v = "  \t\n+2.5"; CHECK_BITS_F(tc_p__strtof_l(v, &e, loc), 2.5f); CHECK_EQ_P(e, v + 8);
    v = " -4";        CHECK_BITS_F(tc_p__strtof_l(v, NULL, loc), -4.0f);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_strtof_l);
    if (!tc_p__strtof_l) { tc_write("TC FAIL " TC_TEST_ID "::__strtof_l not exported\n"); return 1; }
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(simple); TC_RUN(nearest); TC_RUN(exponent); TC_RUN(limits); TC_RUN(endptr); TC_RUN(whitespace_sign);
    _free_locale(loc);
    return tc_end();
}

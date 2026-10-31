/* TC
 * id:    gst_strtof_l_neg_special_legacy_probe
 * area:  conv
 * funcs: _create_locale _free_locale
 * procs: _strtof_l
 * crt:   msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   pre-UCRT strtod grammar (no C99 inf/nan/hex)
 * note:  Probes that pre-UCRT CRTs do NOT parse C99 'inf', 'nan' or hex floats in _strtof_l: expect 0 and endptr==nptr for inf/nan; '0x10' -> 0 with endptr after '0'. msvcr120 is excluded (it may support them).
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

TC_DECL_PROC(float, _strtof_l, (const char *, char **, _locale_t));
static _locale_t loc;
static const char *volatile v;

TC_CASE(inf_not_recognised) { char *e = (char *)1; v = "inf"; CHECK_EQ_U(tc_fbits(tc_p__strtof_l(v, &e, loc)), 0u); CHECK_EQ_P(e, v);
                              v = "infinity"; e = (char *)1; CHECK_EQ_U(tc_fbits(tc_p__strtof_l(v, &e, loc)), 0u); CHECK_EQ_P(e, v); }
TC_CASE(nan_not_recognised) { char *e = (char *)1; v = "nan"; CHECK_EQ_U(tc_fbits(tc_p__strtof_l(v, &e, loc)), 0u); CHECK_EQ_P(e, v); }
TC_CASE(hex_not_recognised) { char *e = (char *)1; v = "0x10"; CHECK_EQ_U(tc_fbits(tc_p__strtof_l(v, &e, loc)), 0u); CHECK_EQ_P(e, v + 1); }
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_strtof_l);
    if (!tc_p__strtof_l) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID " _strtof_l not exported by this CRT\n"); return tc_end(); }
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(inf_not_recognised); TC_RUN(nan_not_recognised); TC_RUN(hex_not_recognised);
    _free_locale(loc);
    return tc_end();
}

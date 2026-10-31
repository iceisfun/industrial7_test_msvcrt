/* TC
 * id:    gst_strtof_l_neg_range_legacy_probe
 * area:  conv
 * funcs: _create_locale _free_locale
 * procs: _strtof_l
 * crt:   msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strtof_l (same text as strtod)
 * note:  Probes whether older CRTs return +/-inf and set ERANGE on _strtof_l overflow/underflow (expected: yes) and that NULL nptr reports EINVAL (msvcr100+ only; msvcr90 skipped).
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <errno.h>

TC_DECL_PROC(float, _strtof_l, (const char *, char **, _locale_t));
static _locale_t loc;
static const char *volatile v;

TC_CASE(overflow) { errno = 0; CHECK_EQ_U(tc_fbits(tc_p__strtof_l(v = "1e39", NULL, loc)), 0x7f800000u); CHECK_EQ_I(errno, ERANGE);
                    errno = 0; CHECK_EQ_U(tc_fbits(tc_p__strtof_l(v = "-1e39", NULL, loc)), 0xff800000u); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(underflow) { errno = 0; CHECK_EQ_U(tc_fbits(tc_p__strtof_l(v = "1e-60", NULL, loc)), 0u); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(null_nptr)
{
    float f;
    if (!TC_AT_LEAST(100) || !tc_iph_install()) TC_SKIP("no invalid parameter handler");
    tc_iph_count = 0; errno = 0;
    v = NULL; f = tc_p__strtof_l(v, NULL, loc);
    CHECK_EQ_U(tc_fbits(f), 0u); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_strtof_l);
    if (!tc_p__strtof_l) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID " _strtof_l not exported by this CRT\n"); return tc_end(); }
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(overflow); TC_RUN(underflow); TC_RUN(null_nptr);
    _free_locale(loc);
    return tc_end();
}

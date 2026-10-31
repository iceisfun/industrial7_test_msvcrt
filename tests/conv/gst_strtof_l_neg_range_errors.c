/* TC
 * id:    gst_strtof_l_neg_range_errors
 * area:  conv
 * funcs: _strtof_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   C11 7.22.1.3p10; MS docs strtof (HUGE_VAL on overflow, 0 on underflow, errno ERANGE); Parameter validation (NULL string: EINVAL, handler)
 * note:  NULL-nptr case runs on ucrtbase only; msvcr120 is probed in gst_strtof_family_neg_null_probe.
 *        Pre-msvcr120 behaviour is split to gst_strtof_l_neg_range_legacy_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <errno.h>

static _locale_t loc;
static const char *volatile v;

TC_CASE(overflow_pos) { errno = 0; CHECK_EQ_U(tc_fbits(_strtof_l(v = "1e39", NULL, loc)), 0x7f800000u); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(overflow_neg) { errno = 0; CHECK_EQ_U(tc_fbits(_strtof_l(v = "-1e39", NULL, loc)), 0xff800000u); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(overflow_endptr) { char *e = NULL; v = "1e39z"; (void)_strtof_l(v, &e, loc); CHECK_EQ_P(e, v + 4); }
TC_CASE(underflow) { errno = 0; CHECK_EQ_U(tc_fbits(_strtof_l(v = "1e-60", NULL, loc)), 0u); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(null_nptr)
{
    char *e = (char *)1; float f;
    if (!TC_AT_LEAST(140)) TC_SKIP("msvcr120 NULL handling is in gst_strtof_family_neg_null_probe");
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler");
    tc_iph_count = 0; errno = 0;
    v = NULL; f = _strtof_l(v, &e, loc);
    CHECK_EQ_U(tc_fbits(f), 0u); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(overflow_pos); TC_RUN(overflow_neg); TC_RUN(overflow_endptr); TC_RUN(underflow); TC_RUN(null_nptr);
    _free_locale(loc);
    return tc_end();
}

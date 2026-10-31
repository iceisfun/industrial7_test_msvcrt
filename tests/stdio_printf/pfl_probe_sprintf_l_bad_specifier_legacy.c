/* TC
 * id:    pfl_probe_sprintf_l_bad_specifier_legacy
 * area:  stdio_printf
 * kind:  positive
 * funcs: _sprintf_l _create_locale _free_locale
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs "Parameter validation", "printf Type Field Characters"
 * note:  PROBE: how legacy (pre-UCRT, VS2008-2013) CRTs treat an unknown conversion character (%y) and a lone trailing '%'.
 *        Most likely: invalid-parameter handler runs, return -1, errno EINVAL. This test only collects ground truth.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <locale.h>

static _locale_t loc;

TC_CASE(pct_y_ret)
{
    char b[32]; int n; volatile const char *f = "a%yb";
    errno = 0; tc_iph_count = 0;
    n = _sprintf_l(b, (const char *)f, loc, 1);
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(pct_trailing_ret)
{
    char b[32]; int n; volatile const char *f = "abc%";
    errno = 0; tc_iph_count = 0;
    n = _sprintf_l(b, (const char *)f, loc);
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(iph_count_observed)
{
    char b[32]; volatile const char *f = "a%yb";
    tc_iph_count = 0;
    _sprintf_l(b, (const char *)f, loc, 1);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::main _create_locale(C) failed\n"); return 1; }
    TC_RUN(pct_y_ret); TC_RUN(pct_trailing_ret); TC_RUN(iph_count_observed);
    _free_locale(loc);
    return tc_end();
}

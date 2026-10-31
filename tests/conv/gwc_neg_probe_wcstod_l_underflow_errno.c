/* TC
 * id:    gwc_neg_probe_wcstod_l_underflow_errno
 * area:  conv
 * funcs: _wcstod_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strtod (ERANGE on underflow); C11 7.22.1.3 (implementation-defined whether errno is set)
 * note:  PROBES: _wcstod_l("1e-999") and ("-1e-999") set errno to ERANGE and return zero of the right sign.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;

TC_CASE(underflow)
{
    wchar_t *e; double r; errno = 0; vp = L"1e-999"; r = _wcstod_l(vp, &e, L);
    CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(underflow_neg)
{
    wchar_t *e; double r; errno = 0; vp = L"-1e-999"; r = _wcstod_l(vp, &e, L);
    CHECK_BITS_D(r, -0.0); CHECK_EQ_I(errno, ERANGE);
}
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(underflow); TC_RUN(underflow_neg);
    _free_locale(L);
    return tc_end();
}

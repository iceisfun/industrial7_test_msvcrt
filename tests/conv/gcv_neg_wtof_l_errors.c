/* TC
 * id:    gcv_neg_wtof_l_errors
 * area:  conv
 * funcs: _wtof_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs atof, _atof_l, _wtof, _wtof_l (HUGE_VAL on overflow, 0 on no conversion, NULL: invalid parameter, EINVAL)
 * note:  Underflow errno not asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <errno.h>
#include <math.h>

static _locale_t L;
static double F(const wchar_t *volatile s) { return _wtof_l(s, L); }

TC_CASE(overflow) { CHECK_BITS_D(F(L"1e999"), HUGE_VAL); CHECK_BITS_D(F(L"-1e999"), -HUGE_VAL); }
TC_CASE(no_conversion)
{
    tc_iph_count = 0;
    CHECK_BITS_D(F(L"abc"), 0.0); CHECK_BITS_D(F(L""), 0.0); CHECK_BITS_D(F(L"e5"), 0.0);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(null_str)
{
    const wchar_t *volatile s = NULL; double r;
    tc_iph_count = 0; errno = 0;
    r = _wtof_l(s, L);
    CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    errno = 0;
    r = _wtof_l(s, NULL);
    CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(underflow) { CHECK_BITS_D(F(L"1e-999"), 0.0); }

int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    L = _create_locale(LC_ALL, "C");
    CHECK(L != NULL);
    if (!L) return tc_end() | 1;
    TC_RUN(overflow); TC_RUN(no_conversion); TC_RUN(null_str); TC_RUN(underflow);
    _free_locale(L);
    return tc_end();
}

/* TC
 * id:    cv_neg_wtof_overflow
 * area:  conv
 * funcs: _wtof _wtoi _wtol
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs atof, _wtof ("+/-HUGE_VAL on overflow, 0.0 if not convertible"); MS docs atoi, _wtoi, _atoi64, (INT_MAX/INT_MIN on overflow in UCRT)
 */
#include "tc.h"
#include <wchar.h>
#include <stdlib.h>
#include <limits.h>

static const wchar_t *volatile v_s;
static volatile double v_zero = 0.0, v_one = 1.0;

TC_CASE(wtof_overflow)
{
    double inf = v_one / v_zero;
    v_s = L"1e999"; CHECK_BITS_D(_wtof(v_s), inf);
    v_s = L"-1e999"; CHECK_BITS_D(_wtof(v_s), -inf);
}
TC_CASE(wtof_garbage)
{
    v_s = L"abc"; CHECK_BITS_D(_wtof(v_s), 0.0);
    v_s = L""; CHECK_BITS_D(_wtof(v_s), 0.0);
}
TC_CASE(wtoi_wtol_clamp)
{
    v_s = L"4294967296"; CHECK_EQ_I(_wtoi(v_s), INT_MAX);
    v_s = L"4294967296"; CHECK_EQ_I(_wtol(v_s), LONG_MAX);
    v_s = L"-4294967296"; CHECK_EQ_I(_wtoi(v_s), INT_MIN);
    v_s = L"-4294967296"; CHECK_EQ_I(_wtol(v_s), LONG_MIN);
}

int main(void)
{
    tc_begin();
    TC_RUN(wtof_overflow); TC_RUN(wtof_garbage); TC_RUN(wtoi_wtol_clamp);
    return tc_end();
}

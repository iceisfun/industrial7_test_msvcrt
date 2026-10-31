/* TC
 * id:    cv_wtof_basic
 * area:  conv
 * funcs: _wtof
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs atof, _atof_l, _wtof, _wtof_l: HUGE_VAL on overflow, 0 if no conversion
 */
#include "tc.h"
#include <stdlib.h>
#include <math.h>

static double W(const wchar_t *volatile s) { return _wtof(s); }

TC_CASE(simple) { CHECK_BITS_D(W(L"1.5"), 1.5); CHECK_BITS_D(W(L"-2e2"), -200.0); CHECK_BITS_D(W(L"0.25"), 0.25); }
TC_CASE(ws) { CHECK_BITS_D(W(L"  \t 3.5xyz"), 3.5); }
TC_CASE(no_digits) { CHECK_BITS_D(W(L"abc"), 0.0); CHECK_BITS_D(W(L""), 0.0); }
TC_CASE(overflow) { CHECK(W(L"1e999") == HUGE_VAL); CHECK(W(L"-1e999") == -HUGE_VAL); }
#ifdef TC_UCRT
TC_CASE(inf_ucrt) { CHECK(W(L"inf") == HUGE_VAL); CHECK(W(L"-infinity") == -HUGE_VAL); }
#endif

int main(void)
{
    tc_begin(); TC_RUN(simple); TC_RUN(ws); TC_RUN(no_digits); TC_RUN(overflow);
#ifdef TC_UCRT
    TC_RUN(inf_ucrt);
#endif
    return tc_end();
}

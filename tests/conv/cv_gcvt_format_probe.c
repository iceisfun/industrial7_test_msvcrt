/* TC
 * id:    cv_gcvt_format_probe
 * area:  conv
 * funcs: _gcvt
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _gcvt
 * note:  Probes exponent width (ucrt 2 digits 'e+10'; legacy 3 digits 'e+010' assumed) and trailing-dot form ('100.'). Record exact text per CRT.
 */
#include "tc.h"
#include <stdlib.h>

static volatile double v_d;

TC_CASE(exp_1e10) { char b[32]; v_d = 1e10; _gcvt(v_d, 3, b);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "1e+10");
#else
    CHECK_STR(b, "1e+010");
#endif
}
TC_CASE(exp_1e_5) { char b[32]; v_d = 1e-5; _gcvt(v_d, 3, b);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "1e-05");
#else
    CHECK_STR(b, "1e-005");
#endif
}
TC_CASE(hundred_trailing_dot) { char b[32]; v_d = 100.0; _gcvt(v_d, 3, b); CHECK_STR(b, "100."); }
TC_CASE(one_digits10) { char b[32]; v_d = 1.0; _gcvt(v_d, 10, b); CHECK_STR(b, "1."); }
TC_CASE(zero) { char b[32]; v_d = 0.0; _gcvt(v_d, 5, b); CHECK_STR(b, "0"); }
TC_CASE(neg_zero) { char b[32]; v_d = -0.0; _gcvt(v_d, 5, b); CHECK_STR(b, "-0"); }

int main(void)
{
    tc_begin();
    TC_RUN(exp_1e10); TC_RUN(exp_1e_5); TC_RUN(hundred_trailing_dot); TC_RUN(one_digits10); TC_RUN(zero); TC_RUN(neg_zero);
    return tc_end();
}

/* TC
 * id:    cv_gcvt_format_probe
 * area:  conv
 * funcs: _gcvt
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _gcvt
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice): _gcvt(1e10,3) -> "1.e+010", _gcvt(1e-5,3) -> "1.e-005", _gcvt(100,3) -> "100",
 *        _gcvt(1,10) -> "1.", _gcvt(0,5) -> "0.", _gcvt(-0.0,5) -> "-0.". The original probe guessed 2-digit exponents and "100."/"0"; real
 *        ucrtbase _gcvt keeps a 3-digit exponent and a bare '.' after a single mantissa digit. Pre-UCRT branches are unverified guesses.
 */
#include "tc.h"
#include <stdlib.h>

static volatile double v_d;

TC_CASE(exp_1e10) { char b[32]; v_d = 1e10; _gcvt(v_d, 3, b);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "1.e+010");   /* observed Win10 22H2 ucrtbase: trailing '.' and 3-digit exponent */
#else
    CHECK_STR(b, "1e+010");
#endif
}
TC_CASE(exp_1e_5) { char b[32]; v_d = 1e-5; _gcvt(v_d, 3, b);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "1.e-005");   /* observed Win10 22H2 ucrtbase */
#else
    CHECK_STR(b, "1e-005");
#endif
}
TC_CASE(hundred_trailing_dot) { char b[32]; v_d = 100.0; _gcvt(v_d, 3, b);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "100");   /* observed Win10 22H2 ucrtbase: no trailing dot when the value fills the digits */
#else
    CHECK_STR(b, "100.");
#endif
}
TC_CASE(one_digits10) { char b[32]; v_d = 1.0; _gcvt(v_d, 10, b); CHECK_STR(b, "1."); }
TC_CASE(zero) { char b[32]; v_d = 0.0; _gcvt(v_d, 5, b);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "0.");   /* observed Win10 22H2 ucrtbase */
#else
    CHECK_STR(b, "0");
#endif
}
TC_CASE(neg_zero) { char b[32]; v_d = -0.0; _gcvt(v_d, 5, b);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "-0.");  /* observed Win10 22H2 ucrtbase */
#else
    CHECK_STR(b, "-0");
#endif
}

int main(void)
{
    tc_begin();
    TC_RUN(exp_1e10); TC_RUN(exp_1e_5); TC_RUN(hundred_trailing_dot); TC_RUN(one_digits10); TC_RUN(zero); TC_RUN(neg_zero);
    return tc_end();
}

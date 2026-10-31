/* TC
 * id:    gmb_chgsign_basic
 * area:  math
 * funcs: _chgsign
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _chgsign
 */
#include "tc.h"
#include <float.h>

static volatile double v_zero = 0.0;

TC_CASE(simple) { volatile double a = 1.5, b = -1.5; CHECK_BITS_D(_chgsign(a), -1.5); CHECK_BITS_D(_chgsign(b), 1.5); }
TC_CASE(zeros) { volatile double a = 0.0, b = -0.0; CHECK_BITS_D(_chgsign(a), -0.0); CHECK_BITS_D(_chgsign(b), 0.0); CHECK_EQ_U(tc_dbits(_chgsign(a)), 0x8000000000000000ULL); }
TC_CASE(extremes)
{
    volatile double m = DBL_MAX, d = 4.9406564584124654e-324;
    CHECK_BITS_D(_chgsign(m), -DBL_MAX);
    CHECK_EQ_U(tc_dbits(_chgsign(d)), 0x8000000000000001ULL);
}
TC_CASE(infinities)
{
    volatile double i = 1.0 / v_zero;
    CHECK_EQ_U(tc_dbits(_chgsign(i)), 0xFFF0000000000000ULL);
    CHECK_EQ_U(tc_dbits(_chgsign(-i)), 0x7FF0000000000000ULL);
}
TC_CASE(involution) { volatile double x = 1e300; CHECK_BITS_D(_chgsign(_chgsign(x)), 1e300); }
int main(void) { tc_begin(); TC_RUN(simple); TC_RUN(zeros); TC_RUN(extremes); TC_RUN(infinities); TC_RUN(involution); return tc_end(); }

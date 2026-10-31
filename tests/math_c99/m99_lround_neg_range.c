/* TC
 * id:    m99_lround_neg_range
 * area:  math_c99
 * funcs: lround lroundf llround llroundf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.6.7 (domain error, unspecified value)
 * note:  asserts only that calls return and errno stays in {0,EDOM,ERANGE}
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
#define ERR_OK() (errno == 0 || errno == EDOM || errno == ERANGE)

TC_CASE(l_range) { volatile double a = 1e10; volatile long r; errno = 0; r = lround(a); (void)r; CHECK(ERR_OK()); }
TC_CASE(ll_range) { volatile double a = 1e30; volatile long long r; errno = 0; r = llround(a); (void)r; CHECK(ERR_OK()); }
TC_CASE(nan_inf) { volatile double i = v_one / v_zero; volatile double n = i - i; volatile long r; volatile long long q;
    errno = 0; r = lround(n); CHECK(ERR_OK()); errno = 0; r = lround(i); CHECK(ERR_OK()); errno = 0; r = lround(-i); CHECK(ERR_OK());
    errno = 0; q = llround(n); CHECK(ERR_OK()); errno = 0; q = llround(i); CHECK(ERR_OK()); (void)r; (void)q; }
TC_CASE(float_variants) { volatile float i = f_one / f_zero; volatile float n = i - i; volatile float big = 1e30f; volatile long r; volatile long long q;
    errno = 0; r = lroundf(big); CHECK(ERR_OK()); errno = 0; r = lroundf(n); CHECK(ERR_OK()); errno = 0; r = lroundf(i); CHECK(ERR_OK());
    errno = 0; q = llroundf(big); CHECK(ERR_OK()); errno = 0; q = llroundf(n); CHECK(ERR_OK()); (void)r; (void)q; }
TC_CASE(still_sane) { volatile double a = 2.5; CHECK_EQ_I(lround(a), 3); CHECK_BITS_D(sqrt(4.0 * v_one), 2.0); }

int main(void) { tc_begin(); TC_RUN(l_range); TC_RUN(ll_range); TC_RUN(nan_inf); TC_RUN(float_variants); TC_RUN(still_sane); return tc_end(); }

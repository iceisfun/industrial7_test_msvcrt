/* TC
 * id:    m99_neg_log2_exp2_nonfinite
 * area:  math_c99
 * funcs: log2 log2f exp2 exp2f
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.3.2 (exp2), F.10.3.11 (log2); MS docs log2, exp2
 * note:  subnormal exact results are asserted with a tiny tolerance (not last-ulp); exact halfway exp2 cases are in m99_neg_exp2_halfway_probe; errno-on-NaN is in m99_neg_log2_exp2_nan_errno_probe (the standard does not say errno must stay unchanged).
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
#define DINF (v_one / v_zero)
#define DNAN (v_zero / v_zero)
#define ERR_OK() (errno == 0 || errno == ERANGE || errno == EDOM)

TC_CASE(log2_nonfinite)
{
    volatile double inf = DINF, n = DNAN, nz = -0.0, one = 1.0; double r;
    errno = 0; CHECK_BITS_D(log2(inf), inf); CHECK(ERR_OK());
    r = log2(n); CHECK(r != r);
    errno = 0; CHECK_BITS_D(log2(nz), -inf); CHECK(ERR_OK());
    CHECK_BITS_D(log2(one), 0.0);
}
TC_CASE(log2_subnormal)
{
    volatile double d = 4.9406564584124654e-324; volatile float f = 1.4012984643248171e-45f;
    CHECK_NEAR(log2(d), -1074.0, 1e-9);
    CHECK_NEAR(log2f(f), -149.0, 1e-4);
}
TC_CASE(exp2_nonfinite)
{
    volatile double ninf = -DINF, inf = DINF, n = DNAN, nz = -0.0; double r;
    CHECK_BITS_D(exp2(ninf), 0.0);
    errno = 0; CHECK_BITS_D(exp2(inf), inf); CHECK(ERR_OK());
    r = exp2(n); CHECK(r != r);
    CHECK_BITS_D(exp2(nz), 1.0);
}
TC_CASE(float_nonfinite)
{
    volatile float n = (float)DNAN, inf = (float)DINF; float r;
    r = log2f(n); CHECK(r != r);
    r = exp2f(n); CHECK(r != r);
    CHECK_BITS_F(log2f(inf), inf);
    CHECK_BITS_F(exp2f(-inf), 0.0f);
}
int main(void)
{
    tc_begin();
    TC_RUN(log2_nonfinite); TC_RUN(log2_subnormal); TC_RUN(exp2_nonfinite); TC_RUN(float_nonfinite);
    return tc_end();
}

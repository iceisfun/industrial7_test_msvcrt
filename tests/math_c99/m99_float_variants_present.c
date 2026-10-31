/* TC
 * id:    m99_float_variants_present
 * area:  math_c99
 * procs: copysignf
 * funcs: cbrtf truncf roundf lroundf llroundf lrintf llrintf nearbyintf rintf fmaf fmaxf fminf fdimf nextafterf nanf expm1f log1pf log2f exp2f erff erfcf tgammaf ilogbf logbf scalbnf scalblnf remainderf remquof asinhf acoshf atanhf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs math functions (float overloads of C99 functions)
 * note:  Import-table presence is the main assertion (enforced by funcs); values loosely checked.
 */
#include "tc.h"
#include <math.h>
static volatile float a = 2.0f, b = 8.0f, h = 2.5f;
static volatile double dv = 2.0;
TC_DECL_PROC(float, copysignf, (float, float));
static volatile int vi = 3;
static volatile long vl = 3;
TC_CASE(rounding)
{
    CHECK_BITS_F(truncf(h), 2.0f); CHECK_BITS_F(roundf(h), 3.0f); CHECK_BITS_F(rintf(h), 2.0f); CHECK_BITS_F(nearbyintf(h), 2.0f);
    CHECK_EQ_I(lroundf(h), 3); CHECK_EQ_I(llroundf(h), 3); CHECK_EQ_I(lrintf(h), 2); CHECK_EQ_I(llrintf(h), 2);
}
TC_CASE(arith)
{
    CHECK_NEAR(cbrtf(b), 2.0, 1e-6); CHECK_BITS_F(fmaf(a, a, a), 6.0f); CHECK_BITS_F(fmaxf(a, b), 8.0f); CHECK_BITS_F(fminf(a, b), 2.0f);
    CHECK_BITS_F(fdimf(b, a), 6.0f); CHECK_BITS_F(remainderf(b, 3.0f * a / a), -1.0f);
    CHECK(nextafterf(a, b) > a);
}
TC_CASE(nan_is_nan) { TC_LOAD_PROC(copysignf); CHECK(tc_p_copysignf && tc_p_copysignf(a, -b) == -2.0f); float n = nanf(""); CHECK(n != n); }
TC_CASE(exp_log)
{
    CHECK_NEAR(expm1f(0.0f * a), 0.0, 1e-7); CHECK_NEAR(log1pf(0.0f * a), 0.0, 1e-7); CHECK_NEAR(log2f(b), 3.0, 1e-6); CHECK_NEAR(exp2f(a), 4.0, 1e-6);
    CHECK_NEAR(erff(0.0f * a), 0.0, 1e-7); CHECK_NEAR(erfcf(0.0f * a), 1.0, 1e-7); CHECK_NEAR(tgammaf(a + 2.0f), 6.0, 1e-4);
    CHECK_NEAR(asinhf(0.0f * a), 0.0, 1e-7); CHECK_NEAR(acoshf(1.0f + 0.0f * a), 0.0, 1e-7); CHECK_NEAR(atanhf(0.0f * a), 0.0, 1e-7);
}
TC_CASE(scale)
{
    int q = -1; float r;
    CHECK_EQ_I(ilogbf(b), 3); CHECK_BITS_F(logbf(b), 3.0f); CHECK_BITS_F(scalbnf(a, vi), 16.0f); CHECK_BITS_F(scalblnf(a, vl), 16.0f);
    r = remquof(b, 3.0f * a / a, &q); CHECK_BITS_F(r, -1.0f); CHECK_EQ_I(q & 7, 3);
}
int main(void) { tc_begin(); TC_RUN(rounding); TC_RUN(arith); TC_RUN(nan_is_nan); TC_RUN(exp_log); TC_RUN(scale); return tc_end(); }

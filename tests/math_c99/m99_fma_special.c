/* TC
 * id:    m99_fma_special
 * area:  math_c99
 * funcs: fma fmaf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.10.1; MS docs fma
 */
#include "tc.h"
#include <math.h>
#include <float.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
static double d_inf(void) { return v_one / v_zero; }
static double d_nan(void) { return d_inf() - d_inf(); }
static float f_inf(void) { return f_one / f_zero; }
static float f_nan(void) { return f_inf() - f_inf(); }

TC_CASE(invalid_nan)
{
    volatile double z = 0.0, o = 1.0, i = d_inf(), n = d_nan();
    CHECK_IS_NAN(fma(z, i, o)); CHECK_IS_NAN(fma(i, z, n)); CHECK_IS_NAN(fma(i, o, -i));
}
TC_CASE(infinite_results)
{
    volatile double i = d_inf(), one = 1.0, two = 2.0;
    CHECK_BITS_D(fma(i, two, one), d_inf()); CHECK_BITS_D(fma(one, two, i), d_inf());
    CHECK_BITS_D(fma(-i, two, one), -d_inf());
}
TC_CASE(zero_signs)
{
    volatile double pz = 0.0, nz = -0.0, one = 1.0;
    CHECK_BITS_D(fma(pz, one, nz), 0.0);
    CHECK_BITS_D(fma(nz, one, nz), -0.0);
    CHECK_BITS_D(fma(pz, one, pz), 0.0);
}
TC_CASE(max_exact)
{
    volatile double m = DBL_MAX, two = 2.0;
    CHECK_BITS_D(fma(m, two, -m), DBL_MAX);
}
TC_CASE(float_special)
{
    volatile float z = 0.0f, o = 1.0f, i = f_inf(), two = 2.0f, m = FLT_MAX;
    CHECK(fmaf(z, i, o) != fmaf(z, i, o));
    CHECK_BITS_F(fmaf(i, two, o), f_inf());
    CHECK_BITS_F(fmaf(m, two, -m), FLT_MAX);
}
int main(void) { tc_begin(); TC_RUN(invalid_nan); TC_RUN(infinite_results); TC_RUN(zero_signs); TC_RUN(max_exact); TC_RUN(float_special); return tc_end(); }

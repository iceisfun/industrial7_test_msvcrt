/* TC
 * id:    m99_fma_basic
 * area:  math_c99
 * funcs: fma fmaf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.13.1; MS docs fma, fmaf
 * note:  Single-rounding case needs a truly fused multiply-add (exact intermediate); a naive a*b+c gives 0 (double: a*b=1-2^-54 ties to 1.0; float: 1-2^-26 rounds to 1.0f).
 */
#include "tc.h"
#include <math.h>
#include <float.h>

TC_CASE(simple)
{
    volatile double a = 2, b = 3, c = 4, na = -2;
    CHECK_BITS_D(fma(a, b, c), 10.0); CHECK_BITS_D(fma(na, b, c), -2.0);
}
TC_CASE(single_rounding)
{
    volatile double a = 1.0 + 0x1p-27, b = 1.0 - 0x1p-27, c = -1.0;
    CHECK_BITS_D(fma(a, b, c), -0x1p-54);
}
TC_CASE(float_variants)
{
    volatile float a = 2.0f, b = 3.0f, c = 4.0f, h = 0.5f, q = 0.25f;
    CHECK_BITS_F(fmaf(a, b, c), 10.0f); CHECK_BITS_F(fmaf(h, h, q), 0.5f);
}
TC_CASE(float_single_rounding)
{
    volatile float a = 1.0f + 0x1p-13f, b = 1.0f - 0x1p-13f, c = -1.0f;
    CHECK_BITS_F(fmaf(a, b, c), -0x1p-26f);
}
TC_CASE(identity)
{
    volatile double xs[] = { 1.5, -7.25, 1e300, 4.9e-324 }; int i;
    volatile double one = 1.0, zero = 0.0;
    for (i = 0; i < 4; i++) CHECK_BITS_D(fma(xs[i], one, zero), xs[i]);
}
int main(void) { tc_begin(); TC_RUN(simple); TC_RUN(single_rounding); TC_RUN(float_variants); TC_RUN(float_single_rounding); TC_RUN(identity); return tc_end(); }

/* TC
 * id:    m99_neg_nextafter_nan_equal
 * area:  math_c99
 * funcs: 
 * procs: nextafter nextafterf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.11.3, F.10.8.3; MS docs nextafter
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

typedef double (__cdecl *na_t)(double, double); typedef float (__cdecl *naf_t)(float, float);
static na_t p_na; static naf_t p_naf;
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_one = 1.0f;
#define DINF (v_one / v_zero)
#define DNAN (v_zero / v_zero)

TC_CASE(nan_args)
{
    volatile double n = DNAN, one = 1.0; double r;
    r = p_na(n, one); CHECK(r != r);
    r = p_na(one, n); CHECK(r != r);
    r = p_na(n, n); CHECK(r != r);
}
TC_CASE(equal_args)
{
    volatile double a = 1.0, b = DBL_MAX, c = 4.9406564584124654e-324, d = -0.0;
    CHECK_BITS_D(p_na(a, a), 1.0);
    CHECK_BITS_D(p_na(b, b), DBL_MAX);
    CHECK_BITS_D(p_na(c, c), 4.9406564584124654e-324);
    CHECK_BITS_D(p_na(d, d), -0.0);
}
TC_CASE(from_infinity)
{
    volatile double inf = DINF, ninf = -DINF;
    CHECK_BITS_D(p_na(inf, ninf), DBL_MAX);
    CHECK_BITS_D(p_na(ninf, inf), -DBL_MAX);
}
TC_CASE(underflow_to_zero)
{
    volatile double c = 4.9406564584124654e-324, z = 0.0;
    CHECK_EQ_U(tc_dbits(p_na(c, z)), 0x0000000000000000ULL);
    CHECK_EQ_U(tc_dbits(p_na(-c, z)), 0x8000000000000000ULL);
}
TC_CASE(float_variants)
{
    volatile float n = (float)DNAN, one = f_one; float r;
    r = p_naf(n, one); CHECK(r != r);
    r = p_naf(one, n); CHECK(r != r);
    CHECK_BITS_F(p_naf(one, one), 1.0f);
}
int main(void)
{
    tc_begin();
    p_na = (na_t)tc_crt_proc("nextafter"); p_naf = (naf_t)tc_crt_proc("nextafterf");
    TC_RUN(nan_args); TC_RUN(equal_args); TC_RUN(from_infinity); TC_RUN(underflow_to_zero); TC_RUN(float_variants);
    return tc_end();
}

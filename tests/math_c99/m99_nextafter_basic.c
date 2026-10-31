/* TC
 * id:    m99_nextafter_basic
 * area:  math_c99
 * funcs:
 * procs: nextafter nextafterf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.11.3; MS docs nextafter
 */
#include "tc.h"
#include <math.h>
#include <float.h>

typedef double (__cdecl *na_t)(double, double);
typedef float (__cdecl *naf_t)(float, float);
static na_t p_na; static naf_t p_naf;
#define nextafter p_na
#define nextafterf p_naf

TC_CASE(exported) { CHECK(p_na != NULL); CHECK(p_naf != NULL); }
TC_CASE(one_ulp)
{
    volatile double one = 1.0, two = 2.0, zero = 0.0;
    CHECK_BITS_D(nextafter(one, two), 1.0 + DBL_EPSILON);
    CHECK_BITS_D(nextafter(one, zero), 1.0 - DBL_EPSILON / 2);
}
TC_CASE(equal) { volatile double x = 3.5, nz = -0.0; CHECK_BITS_D(nextafter(x, x), 3.5); CHECK_BITS_D(nextafter(nz, nz), -0.0); }
TC_CASE(from_zero)
{
    volatile double z = 0.0, p = 1.0, n = -1.0;
    CHECK_EQ_U(tc_dbits(nextafter(z, p)), 1ULL);
    CHECK_EQ_U(tc_dbits(nextafter(z, n)), 0x8000000000000001ULL);
}
TC_CASE(floats)
{
    volatile float one = 1.0f, two = 2.0f, zero = 0.0f;
    CHECK_BITS_F(nextafterf(one, two), 1.0f + FLT_EPSILON);
    CHECK_BITS_F(nextafterf(one, zero), 1.0f - FLT_EPSILON / 2);
    CHECK_EQ_U(tc_fbits(nextafterf(zero, one)), 1U);
}
int main(void) { tc_begin(); p_na = (na_t)tc_crt_proc("nextafter"); p_naf = (naf_t)tc_crt_proc("nextafterf"); TC_RUN(exported); if (!p_na || !p_naf) return tc_end(); TC_RUN(one_ulp); TC_RUN(equal); TC_RUN(from_zero); TC_RUN(floats); return tc_end(); }

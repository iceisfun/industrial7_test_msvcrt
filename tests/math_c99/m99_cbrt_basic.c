/* TC
 * id:    m99_cbrt_basic
 * area:  math_c99
 * funcs: cbrt cbrtf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.7.1; MS docs cbrt
 * note:  C does not require cbrt to be exact even for perfect cubes, so tolerances are used.
 */
#include "tc.h"
#include <math.h>

TC_CASE(exact) { volatile double a = 27.0, b = -8.0, c = 1.0; CHECK_NEAR(cbrt(a), 3.0, 1e-14); CHECK_NEAR(cbrt(b), -2.0, 1e-14); CHECK_NEAR(cbrt(c), 1.0, 1e-14); }
TC_CASE(small) { volatile double a = 0.001; CHECK_NEAR(cbrt(a), 0.1, 1e-15); }
TC_CASE(floats) { volatile float a = 64.0f, b = -125.0f; CHECK_NEAR(cbrtf(a), 4.0, 1e-5); CHECK_NEAR(cbrtf(b), -5.0, 1e-5); }
TC_CASE(odd_symmetry)
{
    static const double xs[] = { 0.5, 2.0, 10.0, 1234.5, 1e-5, 1e20 }; size_t i;
    for (i = 0; i < sizeof xs / sizeof xs[0]; i++) { volatile double x = xs[i], nx = -xs[i]; { double p = cbrt(x), n = cbrt(nx); CHECK_NEAR(n + p, 0.0, 1e-15 * p); } }
}

int main(void) { tc_begin(); TC_RUN(exact); TC_RUN(small); TC_RUN(floats); TC_RUN(odd_symmetry); return tc_end(); }

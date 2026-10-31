/* TC
 * id:    mf_hypotf_basic
 * area:  math_float
 * funcs: _hypotf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _hypot, _hypotf
 */
#include "tc.h"
#include <math.h>
static volatile float a3 = 3, a4 = 4, a5 = 5, a12 = 12, z = 0, m3 = -3, m4 = -4, x7 = -7.5f;

TC_CASE(exact) { CHECK_BITS_F(_hypotf(a3, a4), 5.0f); CHECK_BITS_F(_hypotf(a5, a12), 13.0f); }
TC_CASE(zero) { CHECK_BITS_F(_hypotf(z, z), 0.0f); }
TC_CASE(negatives) { CHECK_BITS_F(_hypotf(m3, m4), 5.0f); }
TC_CASE(axis) { CHECK_BITS_F(_hypotf(x7, z), 7.5f); CHECK_BITS_F(_hypotf(z, x7), 7.5f); }
int main(void) { tc_begin(); TC_RUN(exact); TC_RUN(zero); TC_RUN(negatives); TC_RUN(axis); return tc_end(); }

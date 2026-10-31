/* TC
 * id:    mf_fmodf_signed_zero_probe
 * area:  math_float
 * funcs: fmodf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  probe
 * ref:   C11 F.10.7.1
 * note:  PROBE: C requires fmod(+-0,y) = +-0 and a zero result from an exact multiple to carry the sign of x.
 *        Older Windows CRTs are not known to preserve the sign; each case isolated.
 */
#include "tc.h"
#include <math.h>

TC_CASE(neg_zero_x) { volatile float n = -0.0f, t = 3.0f; CHECK_BITS_F(fmodf(n, t), -0.0f); }
TC_CASE(neg_exact_multiple) { volatile float b = -6.0f, t = 3.0f; CHECK_BITS_F(fmodf(b, t), -0.0f); }

int main(void) { tc_begin(); TC_RUN(neg_zero_x); TC_RUN(neg_exact_multiple); return tc_end(); }

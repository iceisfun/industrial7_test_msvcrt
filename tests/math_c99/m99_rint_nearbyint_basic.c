/* TC
 * id:    m99_rint_nearbyint_basic
 * area:  math_c99
 * funcs: rint rintf nearbyint nearbyintf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.9.3, 7.12.9.4; MS docs rint, nearbyint
 */
#include "tc.h"
#include <math.h>

TC_CASE(rint_table) { volatile double a = 0.5, b = 1.5, c = 2.5, d = -2.5;
    CHECK_BITS_D(rint(a), 0.0); CHECK_BITS_D(rint(b), 2.0); CHECK_BITS_D(rint(c), 2.0); CHECK_BITS_D(rint(d), -2.0); }
TC_CASE(rint_neg_zero) { volatile double a = -0.4; CHECK_BITS_D(rint(a), -0.0); }
TC_CASE(nearbyint_table) { volatile double a = 0.5, b = 1.5, c = 2.5, d = -2.5, e = -0.4;
    CHECK_BITS_D(nearbyint(a), 0.0); CHECK_BITS_D(nearbyint(b), 2.0); CHECK_BITS_D(nearbyint(c), 2.0); CHECK_BITS_D(nearbyint(d), -2.0); CHECK_BITS_D(nearbyint(e), -0.0); }
TC_CASE(floats) { volatile float a = 3.5f, b = -3.5f; CHECK_BITS_F(rintf(a), 4.0f); CHECK_BITS_F(nearbyintf(b), -4.0f); }
TC_CASE(large) { volatile double a = 9007199254740992.0; CHECK_BITS_D(rint(a), 9007199254740992.0); CHECK_BITS_D(nearbyint(a), 9007199254740992.0); }

int main(void) { tc_begin(); TC_RUN(rint_table); TC_RUN(rint_neg_zero); TC_RUN(nearbyint_table); TC_RUN(floats); TC_RUN(large); return tc_end(); }

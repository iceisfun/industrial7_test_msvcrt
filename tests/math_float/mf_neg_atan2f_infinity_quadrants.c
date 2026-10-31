/* TC
 * id:    mf_neg_atan2f_infinity_quadrants
 * area:  math_float
 * funcs: atan2f
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.1.4; MS docs atan2, atan2f
 * note:  ucrtbase only: pre-UCRT x64 atan2f infinity-operand handling not known to follow C99 Annex F.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;
static float fb(unsigned int u) { union { float f; unsigned int u; } x; x.u = u; return x.f; }
#define SENT 1234

#define PI4 0.78539816339744830962
#define PI  3.14159265358979323846
TC_CASE(pinf_pinf_pi4) { volatile float y = v_inf, x = v_inf; CHECK_NEAR(atan2f(y, x), PI4, 1e-6); }
TC_CASE(pinf_ninf_3pi4) { volatile float y = v_inf, x = -v_inf; CHECK_NEAR(atan2f(y, x), 3 * PI4, 1e-6); }
TC_CASE(ninf_pinf_neg_pi4) { volatile float y = -v_inf, x = v_inf; CHECK_NEAR(atan2f(y, x), -PI4, 1e-6); }
TC_CASE(ninf_ninf_neg_3pi4) { volatile float y = -v_inf, x = -v_inf; CHECK_NEAR(atan2f(y, x), -3 * PI4, 1e-6); }
TC_CASE(y_pos_x_ninf_pi) { volatile float y = 1.0f, x = -v_inf; CHECK_NEAR(atan2f(y, x), PI, 1e-6); }
TC_CASE(y_neg_x_ninf_negpi) { volatile float y = -1.0f, x = -v_inf; CHECK_NEAR(atan2f(y, x), -PI, 1e-6); }
TC_CASE(y_pos_x_pinf_zero) { volatile float y = 1.0f, x = v_inf; CHECK_BITS_F(atan2f(y, x), 0.0f); }
TC_CASE(y_neg_x_pinf_negzero) { volatile float y = -1.0f, x = v_inf; CHECK_BITS_F(atan2f(y, x), -0.0f); }

int main(void)
{
    tc_begin(); v_inf = v_one / v_zero;
    TC_RUN(pinf_pinf_pi4); TC_RUN(pinf_ninf_3pi4); TC_RUN(ninf_pinf_neg_pi4); TC_RUN(ninf_ninf_neg_3pi4);
    TC_RUN(y_pos_x_ninf_pi); TC_RUN(y_neg_x_ninf_negpi); TC_RUN(y_pos_x_pinf_zero); TC_RUN(y_neg_x_pinf_negzero);
    return tc_end();
}

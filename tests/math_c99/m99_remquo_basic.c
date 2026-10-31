/* TC
 * id:    m99_remquo_basic
 * area:  math_c99
 * funcs: remquo remquof
 * crt:   ucrtbase msvcr120
 * tier:  p0

 * conf:  standard
 * ref:   C11 7.12.10.3; MS docs remquo
 * note:  Only the low 3 bits of |quotient| and its sign are asserted (standard guarantees at least 3 bits).
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
static double d_inf(void) { return v_one / v_zero; }
static double d_nan(void) { return d_inf() - d_inf(); }
static float f_inf(void) { return f_one / f_zero; }
static float f_nan(void) { return f_inf() - f_inf(); }

static int low3(int q) { return (q < 0 ? -q : q) & 7; }
TC_CASE(five_three) { volatile double x = 5.0, y = 3.0; int q = 99; CHECK_BITS_D(remquo(x, y, &q), -1.0); CHECK_EQ_I(low3(q), 2); CHECK(q > 0); }
TC_CASE(seven_two_tie) { volatile double x = 7.0, y = 2.0; int q = 99; CHECK_BITS_D(remquo(x, y, &q), -1.0); CHECK_EQ_I(low3(q), 4); CHECK(q > 0); }
TC_CASE(neg_five_three) { volatile double x = -5.0, y = 3.0; int q = 99; CHECK_BITS_D(remquo(x, y, &q), 1.0); CHECK_EQ_I(low3(q), 2); CHECK(q < 0); }
TC_CASE(ten_five) { volatile double x = 10.0, y = 5.0; int q = 99; { double r = remquo(x, y, &q); CHECK(r == 0.0); } CHECK_EQ_I(low3(q), 2); CHECK(q > 0); }
TC_CASE(float_five_three) { volatile float x = 5.0f, y = 3.0f; int q = 99; CHECK_BITS_F(remquof(x, y, &q), -1.0f); CHECK_EQ_I(low3(q), 2); CHECK(q > 0); }
TC_CASE(sign_of_quotient) { static const double xs[] = { 5, -5, 5, -5 }, ys[] = { 3, 3, -3, -3 }; int i;
    for (i = 0; i < 4; i++) { volatile double x = xs[i], y = ys[i]; int q = 0; remquo(x, y, &q); CHECK((q < 0) == ((x < 0) != (y < 0))); CHECK_EQ_I(low3(q), 2); } }
int main(void) { tc_begin(); TC_RUN(five_three); TC_RUN(seven_two_tie); TC_RUN(neg_five_three); TC_RUN(ten_five); TC_RUN(float_five_three); TC_RUN(sign_of_quotient); return tc_end(); }

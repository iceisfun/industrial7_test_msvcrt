/* TC
 * id:    mth_tanh_basic
 * area:  math_basic
 * funcs: tanh
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.5.6, F.10.2.6; MS docs tanh
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <errno.h>

#define PI 3.14159265358979323846
static volatile double v_zero = 0.0, v_one = 1.0, v_inf;
static void init(void) { v_inf = v_one / v_zero; }

TC_CASE(zero) { volatile double z=0.0,n=-0.0; CHECK_BITS_D(tanh(z),0.0); CHECK_BITS_D(tanh(n),-0.0); }
TC_CASE(inf) { volatile double a=v_inf,b=-v_inf; CHECK_BITS_D(tanh(a),1.0); CHECK_BITS_D(tanh(b),-1.0); }
TC_CASE(one) { volatile double a=1.0; CHECK_NEAR(tanh(a),0.7615941559557649,1e-15); }
TC_CASE(twenty) { volatile double a=20.0; CHECK_NEAR(tanh(a),1.0,1e-15); }

int main(void) { tc_begin(); init(); TC_RUN(zero); TC_RUN(inf); TC_RUN(one); TC_RUN(twenty); return tc_end(); }

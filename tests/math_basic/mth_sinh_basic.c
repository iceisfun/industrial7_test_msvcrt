/* TC
 * id:    mth_sinh_basic
 * area:  math_basic
 * funcs: sinh
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.5.5, F.10.2.5; MS docs sinh
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <errno.h>

#define PI 3.14159265358979323846
static volatile double v_zero = 0.0, v_one = 1.0, v_inf;
static void init(void) { v_inf = v_one / v_zero; }

TC_CASE(zero) { volatile double z=0.0,n=-0.0; CHECK_BITS_D(sinh(z),0.0); CHECK_BITS_D(sinh(n),-0.0); }
TC_CASE(one) { volatile double a=1.0; CHECK_NEAR(sinh(a),1.1752011936438014,1e-15); }
TC_CASE(odd) { volatile double x=0.7,y=-0.7; CHECK(sinh(y)==-sinh(x)); }
TC_CASE(inf) { volatile double a=v_inf,b=-v_inf; CHECK(sinh(a)==v_inf); CHECK(sinh(b)==-v_inf); }

int main(void) { tc_begin(); init(); TC_RUN(zero); TC_RUN(one); TC_RUN(odd); TC_RUN(inf); return tc_end(); }

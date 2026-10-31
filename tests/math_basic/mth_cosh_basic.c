/* TC
 * id:    mth_cosh_basic
 * area:  math_basic
 * funcs: cosh
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.5.4, F.10.2.4; MS docs cosh
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <errno.h>

#define PI 3.14159265358979323846
static volatile double v_zero = 0.0, v_one = 1.0, v_inf;
static void init(void) { v_inf = v_one / v_zero; }

TC_CASE(zero) { volatile double z=0.0; CHECK_BITS_D(cosh(z),1.0); }
TC_CASE(one) { volatile double a=1.0; CHECK_NEAR(cosh(a),1.5430806348152437,1e-15); }
TC_CASE(even) { volatile double x=0.7,y=-0.7; CHECK(cosh(y)==cosh(x)); }
TC_CASE(inf) { volatile double a=v_inf,b=-v_inf; CHECK(cosh(a)==v_inf); CHECK(cosh(b)==v_inf); }

int main(void) { tc_begin(); init(); TC_RUN(zero); TC_RUN(one); TC_RUN(even); TC_RUN(inf); return tc_end(); }

/* TC
 * id:    mth_atan2_quadrants
 * area:  math_basic
 * funcs: atan2
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.4.4, F.10.1.4; MS docs atan2
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <errno.h>

#define PI 3.14159265358979323846
static volatile double v_zero = 0.0, v_one = 1.0, v_inf;
static void init(void) { v_inf = v_one / v_zero; }

TC_CASE(q1) { volatile double a=1.0; CHECK_NEAR(atan2(a,a),PI/4,1e-15); }
TC_CASE(q2) { volatile double y=1.0,x=-1.0; CHECK_NEAR(atan2(y,x),3*PI/4,1e-15); }
TC_CASE(q3) { volatile double y=-1.0,x=-1.0; CHECK_NEAR(atan2(y,x),-3*PI/4,1e-15); }
TC_CASE(q4) { volatile double y=-1.0,x=1.0; CHECK_NEAR(atan2(y,x),-PI/4,1e-15); }
TC_CASE(axes) { volatile double p=1.0,n=-1.0,z=0.0; CHECK_NEAR(atan2(p,z),PI/2,1e-15); CHECK_NEAR(atan2(n,z),-PI/2,1e-15); CHECK_NEAR(atan2(z,n),PI,1e-15); CHECK_BITS_D(atan2(z,p),0.0); }

int main(void) { tc_begin(); init(); TC_RUN(q1); TC_RUN(q2); TC_RUN(q3); TC_RUN(q4); TC_RUN(axes); return tc_end(); }

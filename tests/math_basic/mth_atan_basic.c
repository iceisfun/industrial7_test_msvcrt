/* TC
 * id:    mth_atan_basic
 * area:  math_basic
 * funcs: atan
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.4.3, F.10.1.3; MS docs atan
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <errno.h>

#define PI 3.14159265358979323846
static volatile double v_zero = 0.0, v_one = 1.0, v_inf;
static void init(void) { v_inf = v_one / v_zero; }

TC_CASE(zero) { volatile double z=0.0, n=-0.0; CHECK_BITS_D(atan(z),0.0); CHECK_BITS_D(atan(n),-0.0); }
TC_CASE(one) { volatile double a=1.0; CHECK_NEAR(atan(a),PI/4,1e-15); }
TC_CASE(inf) { volatile double a=v_inf, b=-v_inf; CHECK_NEAR(atan(a),PI/2,1e-15); CHECK_NEAR(atan(b),-PI/2,1e-15); }
TC_CASE(odd) { volatile double x[3]={0.3,1.0,25.0}; int i; for(i=0;i<3;i++){ volatile double y=-x[i]; CHECK(atan(y)==-atan(x[i])); } }

int main(void) { tc_begin(); init(); TC_RUN(zero); TC_RUN(one); TC_RUN(inf); TC_RUN(odd); return tc_end(); }

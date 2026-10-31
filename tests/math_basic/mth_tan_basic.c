/* TC
 * id:    mth_tan_basic
 * area:  math_basic
 * funcs: tan
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.4.7, F.10.1.7; MS docs tan
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <errno.h>

#define PI 3.14159265358979323846
static volatile double v_zero = 0.0, v_one = 1.0, v_inf;
static void init(void) { v_inf = v_one / v_zero; }

TC_CASE(zero) { volatile double z=0.0, n=-0.0; CHECK_BITS_D(tan(z),0.0); CHECK_BITS_D(tan(n),-0.0); }
TC_CASE(quarter_pi) { volatile double a=PI/4, b=-PI/4; CHECK_NEAR(tan(a),1.0,1e-15); CHECK_NEAR(tan(b),-1.0,1e-15); }
TC_CASE(odd) { volatile double x[3]={0.3,1.0,1.4}; int i; for(i=0;i<3;i++){ volatile double y=-x[i]; CHECK(tan(x[i])==-tan(y)); } }

int main(void) { tc_begin(); init(); TC_RUN(zero); TC_RUN(quarter_pi); TC_RUN(odd); return tc_end(); }

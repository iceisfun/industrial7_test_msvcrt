/* TC
 * id:    mth_sin_basic
 * area:  math_basic
 * funcs: sin
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.4.6, F.10.1.6; MS docs sin
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <errno.h>

#define PI 3.14159265358979323846
static volatile double v_zero = 0.0, v_one = 1.0, v_inf;
static void init(void) { v_inf = v_one / v_zero; }

TC_CASE(zero) { volatile double z=0.0, n=-0.0; CHECK_BITS_D(sin(z),0.0); CHECK_BITS_D(sin(n),-0.0); }
TC_CASE(known) { volatile double a=PI/2, b=PI/6, c=PI; CHECK_NEAR(sin(a),1.0,1e-15); CHECK_NEAR(sin(b),0.5,1e-15); CHECK_NEAR(sin(c),0.0,2e-16); }
TC_CASE(odd) { volatile double x[3]={0.3,1.0,2.5}; int i; for(i=0;i<3;i++){ volatile double y=-x[i]; CHECK(sin(y)==-sin(x[i])); } }

int main(void) { tc_begin(); init(); errno = 0; /* references _errno so the import library, not libmingwex, supplies sin/cos */ TC_RUN(zero); TC_RUN(known); TC_RUN(odd); return tc_end(); }

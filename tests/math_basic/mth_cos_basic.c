/* TC
 * id:    mth_cos_basic
 * area:  math_basic
 * funcs: cos
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.4.5, F.10.1.5; MS docs cos
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <errno.h>

#define PI 3.14159265358979323846
static volatile double v_zero = 0.0, v_one = 1.0, v_inf;
static void init(void) { v_inf = v_one / v_zero; }

TC_CASE(one) { volatile double z=0.0, n=-0.0; CHECK_BITS_D(cos(z),1.0); CHECK_BITS_D(cos(n),1.0); }
TC_CASE(known) { volatile double a=PI, b=PI/3, c=PI/2; CHECK_NEAR(cos(a),-1.0,1e-15); CHECK_NEAR(cos(b),0.5,1e-15); CHECK_NEAR(cos(c),0.0,1e-16); }
TC_CASE(even) { volatile double x[3]={0.3,1.0,2.5}; int i; for(i=0;i<3;i++){ volatile double y=-x[i]; CHECK(cos(y)==cos(x[i])); } }

int main(void) { tc_begin(); init(); errno = 0; /* references _errno so the import library, not libmingwex, supplies sin/cos */ TC_RUN(one); TC_RUN(known); TC_RUN(even); return tc_end(); }

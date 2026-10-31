/* TC
 * id:    mth_atan2_signed_zero
 * area:  math_basic
 * funcs: atan2
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.1.4; MS docs atan2
 * note:  no errno asserted
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <errno.h>

#define PI 3.14159265358979323846
static volatile double v_zero = 0.0, v_one = 1.0, v_inf;
static void init(void) { v_inf = v_one / v_zero; }

TC_CASE(zero_zero) { volatile double pz=0.0,nz=-0.0; CHECK_NEAR(atan2(pz,nz),PI,1e-15); CHECK_NEAR(atan2(nz,nz),-PI,1e-15); CHECK_BITS_D(atan2(nz,pz),-0.0); CHECK_BITS_D(atan2(pz,pz),0.0); }
TC_CASE(zero_neg_x) { volatile double pz=0.0,nz=-0.0,m=-1.0; CHECK_NEAR(atan2(pz,m),PI,1e-15); CHECK_NEAR(atan2(nz,m),-PI,1e-15); }
TC_CASE(finite_inf) { volatile double p=1.0,n=-1.0,i=v_inf,ni=-v_inf; CHECK_BITS_D(atan2(p,i),0.0); CHECK_BITS_D(atan2(n,i),-0.0); CHECK_NEAR(atan2(p,ni),PI,1e-15); CHECK_NEAR(atan2(n,ni),-PI,1e-15); }
TC_CASE(inf_inf) { volatile double i=v_inf; CHECK_NEAR(atan2(i,i),PI/4,1e-15); }

int main(void) { tc_begin(); init(); TC_RUN(zero_zero); TC_RUN(zero_neg_x); TC_RUN(finite_inf); TC_RUN(inf_inf); return tc_end(); }

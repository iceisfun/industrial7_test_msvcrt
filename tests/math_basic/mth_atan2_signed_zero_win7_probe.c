/* TC
 * id:    mth_atan2_signed_zero_win7_probe
 * area:  math_basic
 * funcs: atan2
 * crt:   msvcrt msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   C11 F.10.1.4
 * note:  probing: legacy CRT atan2(0,-0)=pi and atan2(-0,-0)=-pi as in ucrt
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <errno.h>

#define PI 3.14159265358979323846
static volatile double v_zero = 0.0, v_one = 1.0, v_inf;
static void init(void) { v_inf = v_one / v_zero; }

TC_CASE(zeros) { volatile double pz=0.0,nz=-0.0; CHECK_NEAR(atan2(pz,nz),PI,1e-15); CHECK_NEAR(atan2(nz,nz),-PI,1e-15); }
TC_CASE(inf) { volatile double p=1.0,i=v_inf,ni=-v_inf; CHECK_NEAR(atan2(p,ni),PI,1e-15); CHECK_NEAR(atan2(i,i),PI/4,1e-15); }

int main(void) { tc_begin(); init(); TC_RUN(zeros); TC_RUN(inf); return tc_end(); }

/* TC
 * id:    mth_acos_basic
 * area:  math_basic
 * funcs: acos
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.4.1, F.10.1.1; MS docs acos
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <errno.h>

#define PI 3.14159265358979323846
static volatile double v_zero = 0.0, v_one = 1.0, v_inf;
static void init(void) { v_inf = v_one / v_zero; }

TC_CASE(one) { volatile double a=1.0; CHECK_BITS_D(acos(a),0.0); }
TC_CASE(known) { volatile double a=-1.0, b=0.0, c=0.5; CHECK_NEAR(acos(a),PI,1e-15); CHECK_NEAR(acos(b),PI/2,1e-15); CHECK_NEAR(acos(c),PI/3,1e-15); }

int main(void) { tc_begin(); init(); TC_RUN(one); TC_RUN(known); return tc_end(); }

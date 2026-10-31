/* TC
 * id:    mth_asin_basic
 * area:  math_basic
 * funcs: asin
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.4.2, F.10.1.2; MS docs asin
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <errno.h>

#define PI 3.14159265358979323846
static volatile double v_zero = 0.0, v_one = 1.0, v_inf;
static void init(void) { v_inf = v_one / v_zero; }

TC_CASE(zero) { volatile double z=0.0, n=-0.0; CHECK_BITS_D(asin(z),0.0); CHECK_BITS_D(asin(n),-0.0); }
TC_CASE(ends) { volatile double a=1.0, b=-1.0; CHECK_NEAR(asin(a),PI/2,1e-15); CHECK_NEAR(asin(b),-PI/2,1e-15); }
TC_CASE(half) { volatile double a=0.5; CHECK_NEAR(asin(a),PI/6,1e-15); }

int main(void) { tc_begin(); init(); TC_RUN(zero); TC_RUN(ends); TC_RUN(half); return tc_end(); }

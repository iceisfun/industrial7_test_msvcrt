/* TC
 * id:    mth_neg_sqrt_log_domain
 * area:  math_basic
 * funcs: sqrt log log10
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sqrt, log, log10 (EDOM)
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

#define PI 3.14159265358979323846
static volatile double v_zero = 0.0, v_one = 1.0, v_inf;
static void init(void) { v_inf = v_one / v_zero; }

TC_CASE(sqrt) { volatile double a=-1.0; errno=0; CHECK_IS_NAN(sqrt(a)); CHECK_EQ_I(errno,EDOM); }
TC_CASE(log) { volatile double a=-1.0; errno=0; CHECK_IS_NAN(log(a)); CHECK_EQ_I(errno,EDOM); }
TC_CASE(log10) { volatile double a=-1.0; errno=0; CHECK_IS_NAN(log10(a)); CHECK_EQ_I(errno,EDOM); }
TC_CASE(valid_no_errno) { volatile double a=4.0; errno=0; CHECK_BITS_D(sqrt(a),2.0); CHECK_NEAR(log10(100.0*v_one),2.0,1e-15); CHECK_EQ_I(errno,0); }

int main(void) { tc_begin(); init(); TC_RUN(sqrt); TC_RUN(log); TC_RUN(log10); TC_RUN(valid_no_errno); return tc_end(); }

/* TC
 * id:    mth_neg_asin_acos_domain
 * area:  math_basic
 * funcs: asin acos
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs asin, acos (EDOM)
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

#define PI 3.14159265358979323846
static volatile double v_zero = 0.0, v_one = 1.0, v_inf;
static void init(void) { v_inf = v_one / v_zero; }

TC_CASE(asin) { volatile double a=2.0,b=-1.5; errno=0; CHECK_IS_NAN(asin(a)); CHECK_EQ_I(errno,EDOM); errno=0; CHECK_IS_NAN(asin(b)); CHECK_EQ_I(errno,EDOM); }
TC_CASE(acos) { volatile double a=2.0,b=-1.5; errno=0; CHECK_IS_NAN(acos(a)); CHECK_EQ_I(errno,EDOM); errno=0; CHECK_IS_NAN(acos(b)); CHECK_EQ_I(errno,EDOM); }
TC_CASE(in_domain_no_errno) { volatile double a=0.5; errno=0; (void)asin(a); (void)acos(a); CHECK_EQ_I(errno,0); }

int main(void) { tc_begin(); init(); TC_RUN(asin); TC_RUN(acos); TC_RUN(in_domain_no_errno); return tc_end(); }

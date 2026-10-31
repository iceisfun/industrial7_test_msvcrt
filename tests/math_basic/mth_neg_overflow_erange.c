/* TC
 * id:    mth_neg_overflow_erange
 * area:  math_basic
 * funcs: exp sinh cosh
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs exp, sinh, cosh (ERANGE on overflow)
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

#define PI 3.14159265358979323846
static volatile double v_zero = 0.0, v_one = 1.0, v_inf;
static void init(void) { v_inf = v_one / v_zero; }

TC_CASE(exp) { volatile double a=1000.0; errno=0; CHECK(exp(a)==v_inf); CHECK_EQ_I(errno,ERANGE); }
TC_CASE(sinh) { volatile double a=1000.0,b=-1000.0; errno=0; CHECK(sinh(a)==v_inf); CHECK_EQ_I(errno,ERANGE); errno=0; CHECK(sinh(b)==-v_inf); CHECK_EQ_I(errno,ERANGE); }
TC_CASE(cosh) { volatile double a=1000.0; errno=0; CHECK(cosh(a)==v_inf); CHECK_EQ_I(errno,ERANGE); }
TC_CASE(no_overflow) { volatile double a=1.0; errno=0; (void)exp(a); (void)sinh(a); (void)cosh(a); CHECK_EQ_I(errno,0); }

int main(void) { tc_begin(); init(); TC_RUN(exp); TC_RUN(sinh); TC_RUN(cosh); TC_RUN(no_overflow); return tc_end(); }

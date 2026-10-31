/* TC
 * id:    mth_neg_log_zero_erange
 * area:  math_basic
 * funcs: log log10
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs log, log10 (ERANGE, -INF for 0)
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

#define PI 3.14159265358979323846
static volatile double v_zero = 0.0, v_one = 1.0, v_inf;
static void init(void) { v_inf = v_one / v_zero; }

TC_CASE(log0) { volatile double a=0.0; errno=0; CHECK(log(a)==-v_inf); CHECK_EQ_I(errno,ERANGE); }
TC_CASE(log10_0) { volatile double a=0.0; errno=0; CHECK(log10(a)==-v_inf); CHECK_EQ_I(errno,ERANGE); }

int main(void) { tc_begin(); init(); TC_RUN(log0); TC_RUN(log10_0); return tc_end(); }

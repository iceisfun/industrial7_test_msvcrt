/* TC
 * id:    m99_ilogb_logb_special
 * area:  math_c99
 * funcs: ilogb ilogbf logb logbf
 * procs: 
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs ilogb (FP_ILOGB0, FP_ILOGBNAN, INT_MAX for inf); C11 F.10.3.5, F.10.3.11
 * note:  constants defined locally: FP_ILOGB0 = INT_MIN, FP_ILOGBNAN = INT_MAX per MS math.h
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#define MS_ILOGB0 ((int)0x80000000)
#define MS_ILOGBNAN 0x7fffffff
static volatile double v_zero = 0.0, v_one = 1.0;
TC_CASE(ilogb_zero) { volatile double z = -0.0; volatile float f = 0.0f; CHECK_EQ_I(ilogb(v_zero), MS_ILOGB0); CHECK_EQ_I(ilogb(z), MS_ILOGB0); CHECK_EQ_I(ilogbf(f), MS_ILOGB0); }
TC_CASE(ilogb_nan) { volatile double n = (v_one / v_zero) - (v_one / v_zero); CHECK_EQ_I(ilogb(n), MS_ILOGBNAN); }
TC_CASE(ilogb_inf) { volatile double i = v_one / v_zero; CHECK_EQ_I(ilogb(i), INT_MAX); CHECK_EQ_I(ilogb(-i), INT_MAX); }
TC_CASE(logb_inf) { volatile double i = v_one / v_zero; CHECK(logb(i) == i); CHECK(logb(-i) == i); }
TC_CASE(logb_nan) { volatile double n = (v_one / v_zero) - (v_one / v_zero); volatile float fn = (float)n; CHECK_IS_NAN(logb(n)); CHECK_IS_NAN(logbf(fn)); }
int main(void) { tc_begin(); TC_RUN(ilogb_zero); TC_RUN(ilogb_nan); TC_RUN(ilogb_inf); TC_RUN(logb_inf); TC_RUN(logb_nan); return tc_end(); }

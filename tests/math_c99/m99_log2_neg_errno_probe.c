/* TC
 * id:    m99_log2_neg_errno_probe
 * area:  math_c99
 * funcs: log2 exp2
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs log2 / exp2 (errno behaviour); C11 7.12.1
 * note:  probes exact errno on real Windows: log2(-1)->EDOM, log2(0)->ERANGE, exp2(1024)->ERANGE; exp2(-1100) only asserted in {0,ERANGE}.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
#define PINF (v_one / v_zero)
#define QNAN (PINF - PINF)
#define FPINF (f_one / f_zero)
#define FQNAN (FPINF - FPINF)

TC_CASE(log2_neg) { volatile double a = -1.0; errno = 0; (void)log2(a); CHECK_EQ_I(errno, EDOM); }
TC_CASE(log2_zero) { volatile double a = 0.0; errno = 0; (void)log2(a); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(exp2_overflow) { volatile double a = 1024; errno = 0; (void)exp2(a); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(exp2_underflow) { volatile double a = -1100; errno = 0; (void)exp2(a); CHECK(errno == 0 || errno == ERANGE); }

int main(void) { tc_begin(); TC_RUN(log2_neg); TC_RUN(log2_zero); TC_RUN(exp2_overflow); TC_RUN(exp2_underflow); return tc_end(); }

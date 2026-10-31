/* TC
 * id:    m99_tgamma_neg_errno_probe
 * area:  math_c99
 * funcs: tgamma
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.1; MS docs tgamma
 * note:  probes exact errno on real Windows: tgamma(-1)->EDOM, tgamma(0)->ERANGE, tgamma(172)->ERANGE.
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

TC_CASE(neg_one) { volatile double a = -1; errno = 0; (void)tgamma(a); CHECK_EQ_I(errno, EDOM); }
TC_CASE(zero) { volatile double a = 0; errno = 0; (void)tgamma(a); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(overflow) { volatile double a = 172; errno = 0; (void)tgamma(a); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_RUN(neg_one); TC_RUN(zero); TC_RUN(overflow); return tc_end(); }

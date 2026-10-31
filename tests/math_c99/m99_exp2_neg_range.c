/* TC
 * id:    m99_exp2_neg_range
 * area:  math_c99
 * funcs: exp2 exp2f
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.6.4, F.10 (overflow returns +inf, underflow returns 0); errno may be 0 or ERANGE (7.12.1)
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

#define ERR_OK() (errno == 0 || errno == ERANGE)
TC_CASE(overflow) { volatile double a = 1024, b = 1e6; errno = 0; CHECK_BITS_D(exp2(a), PINF); CHECK(ERR_OK()); errno = 0; CHECK_BITS_D(exp2(b), PINF); CHECK(ERR_OK()); }
TC_CASE(underflow) { volatile double a = -1080, b = -2000; errno = 0; CHECK_BITS_D(exp2(a), 0.0); CHECK(ERR_OK()); errno = 0; CHECK_BITS_D(exp2(b), 0.0); CHECK(ERR_OK()); }
TC_CASE(float_range) { volatile float a = 128.0f, b = -200.0f; errno = 0; CHECK_BITS_F(exp2f(a), FPINF); CHECK(ERR_OK()); errno = 0; CHECK_BITS_F(exp2f(b), 0.0f); CHECK(ERR_OK()); }
TC_CASE(just_below_overflow) { volatile double a = 1023.0; CHECK(exp2(a) < PINF); }

int main(void) { tc_begin(); TC_RUN(overflow); TC_RUN(underflow); TC_RUN(float_range); TC_RUN(just_below_overflow); return tc_end(); }

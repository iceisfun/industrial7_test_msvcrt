/* TC
 * id:    m99_tgamma_neg_domain
 * area:  math_c99
 * funcs: tgamma tgammaf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.5.4 (negative integer and -inf: NaN, invalid; overflow: +inf); errno may be 0, EDOM or ERANGE
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

#define ERR_OK() (errno == 0 || errno == EDOM || errno == ERANGE)
TC_CASE(neg_integers) { volatile double a = -1, b = -2; errno = 0; CHECK_IS_NAN(tgamma(a)); CHECK(ERR_OK()); errno = 0; CHECK_IS_NAN(tgamma(b)); CHECK(ERR_OK()); }
TC_CASE(neg_inf) { volatile double i = PINF; errno = 0; CHECK_IS_NAN(tgamma(-i)); CHECK(ERR_OK()); }
TC_CASE(overflow) { volatile double a = 172; errno = 0; CHECK_BITS_D(tgamma(a), PINF); CHECK(ERR_OK()); }
TC_CASE(float_overflow) { volatile float a = 36.0f; errno = 0; CHECK_BITS_F(tgammaf(a), FPINF); CHECK(ERR_OK()); }

int main(void) { tc_begin(); TC_RUN(neg_integers); TC_RUN(neg_inf); TC_RUN(overflow); TC_RUN(float_overflow); return tc_end(); }

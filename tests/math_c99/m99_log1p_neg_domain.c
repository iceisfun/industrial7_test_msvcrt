/* TC
 * id:    m99_log1p_neg_domain
 * area:  math_c99
 * funcs: log1p log1pf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.6.9, F.10.3.9
 * note:  errno constrained to {0,EDOM,ERANGE}
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
#define DINF (v_one / v_zero)
#define FINF (f_one / f_zero)

#define ERR_OK() (errno == 0 || errno == EDOM || errno == ERANGE)

TC_CASE(below_minus1) { volatile double x = -2.0; double r; errno = 0; r = log1p(x); CHECK_IS_NAN(r); CHECK(ERR_OK()); }
TC_CASE(float_below) { volatile float x = -1.5f; float r; errno = 0; r = log1pf(x); CHECK(r != r); CHECK(ERR_OK()); }
TC_CASE(neg_inf) { volatile double x = -DINF; double r; errno = 0; r = log1p(x); CHECK_IS_NAN(r); CHECK(ERR_OK()); }
TC_CASE(pole) { volatile double x = -1.0; double r; errno = 0; r = log1p(x); CHECK_BITS_D(r, -DINF); CHECK(ERR_OK()); }

int main(void) { tc_begin(); TC_RUN(below_minus1); TC_RUN(float_below); TC_RUN(neg_inf); TC_RUN(pole); return tc_end(); }

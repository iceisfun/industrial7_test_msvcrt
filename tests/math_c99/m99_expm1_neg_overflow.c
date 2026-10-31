/* TC
 * id:    m99_expm1_neg_overflow
 * area:  math_c99
 * funcs: expm1 expm1f
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.6.3, F.10.3.4 (overflow gives +HUGE_VAL)
 * note:  errno constrained only to {0,ERANGE}
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
#define DINF (v_one / v_zero)
#define FINF (f_one / f_zero)

#define ERR_OK() (errno == 0 || errno == ERANGE)

TC_CASE(dbl) { volatile double x = 710.0; double r; errno = 0; r = expm1(x); CHECK_BITS_D(r, DINF); CHECK(ERR_OK()); }
TC_CASE(flt) { volatile float x = 100.0f; float r; errno = 0; r = expm1f(x); CHECK_BITS_F(r, FINF); CHECK(ERR_OK()); }
TC_CASE(huge) { volatile double x = 1e300; CHECK_BITS_D(expm1(x), DINF); }

int main(void) { tc_begin(); TC_RUN(dbl); TC_RUN(flt); TC_RUN(huge); return tc_end(); }

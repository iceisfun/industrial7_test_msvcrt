/* TC
 * id:    m99_erfc_neg_underflow
 * area:  math_c99
 * funcs: erfc erfcf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.8.2, 7.12.1 (underflow may set ERANGE); true erfc(27)~5e-319 (denormal), erfc(100)~1e-4346, erfcf(11)~1e-54 (< FLT_TRUE_MIN/2)
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
TC_CASE(d27) { volatile double x = 27; double r; errno = 0; r = erfc(x); CHECK(r >= 0.0 && r <= 1e-300); CHECK(ERR_OK()); }
TC_CASE(d100) { volatile double x = 100; errno = 0; CHECK_BITS_D(erfc(x), 0.0); CHECK(ERR_OK()); }
TC_CASE(f11) { volatile float x = 11; errno = 0; CHECK_BITS_F(erfcf(x), 0.0f); CHECK(ERR_OK()); }
TC_CASE(f100) { volatile float x = 100; errno = 0; CHECK_BITS_F(erfcf(x), 0.0f); CHECK(ERR_OK()); }

int main(void) { tc_begin(); TC_RUN(d27); TC_RUN(d100); TC_RUN(f11); TC_RUN(f100); return tc_end(); }

/* TC
 * id:    m99_log1p_neg_errno_probe
 * area:  math_c99
 * funcs: log1p
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.1 (math_errhandling); MS docs log1p, errno
 * note:  PROBE: UCRT sets errno EDOM for x<-1 and ERANGE for x==-1; msvcr120 may not set errno for C99 functions. msvcr120 is included: a failure there is informative.
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
#define DINF (v_one / v_zero)
#define FINF (f_one / f_zero)

TC_CASE(edom) { volatile double x = -2.0; errno = 0; (void)log1p(x); CHECK_EQ_I(errno, EDOM); }
TC_CASE(erange) { volatile double x = -1.0; errno = 0; (void)log1p(x); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(ok_no_errno) { volatile double x = 1.0; errno = 0; (void)log1p(x); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(edom); TC_RUN(erange); TC_RUN(ok_no_errno); return tc_end(); }

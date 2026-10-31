/* TC
 * id:    m99_acosh_neg_errno_probe
 * area:  math_c99
 * funcs: acosh atanh asinh
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.1 (EDOM for domain, ERANGE for pole may be set); MS docs acosh/atanh
 * note:  PROBE: errno values after domain/pole errors: acosh(0.5)->EDOM, atanh(2)->EDOM, atanh(+-1)->ERANGE, asinh(1) leaves errno untouched. Each in own case.
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
static double d_inf(void) { return v_one / v_zero; }
static double d_nan(void) { return d_inf() - d_inf(); }
static float f_inf(void) { return f_one / f_zero; }

TC_CASE(acosh_dom) { volatile double x = 0.5; errno = 0; acosh(x); CHECK_EQ_I(errno, EDOM); }
TC_CASE(atanh_dom) { volatile double x = 2.0; errno = 0; atanh(x); CHECK_EQ_I(errno, EDOM); }
TC_CASE(atanh_pole) { volatile double x = 1.0, y = -1.0; errno = 0; atanh(x); CHECK_EQ_I(errno, ERANGE); errno = 0; atanh(y); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(asinh_ok) { volatile double x = 1.0; errno = 0; asinh(x); CHECK_EQ_I(errno, 0); }
int main(void) { tc_begin(); TC_RUN(acosh_dom); TC_RUN(atanh_dom); TC_RUN(atanh_pole); TC_RUN(asinh_ok); return tc_end(); }

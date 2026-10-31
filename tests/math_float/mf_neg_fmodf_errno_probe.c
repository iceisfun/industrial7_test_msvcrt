/* TC
 * id:    mf_neg_fmodf_errno_probe
 * area:  math_float
 * funcs: fmodf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.1, F.10.7.1; MS docs fmod
 * note:  PROBE: errno after fmodf(1,0) and fmodf(inf,2). Expected EDOM (domain error); Windows may leave errno untouched.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile float v_zero = 0.0f, v_one = 1.0f;

TC_CASE(div_zero) { volatile float x = 1.0f, y = 0.0f; volatile float r; errno = 0; r = fmodf(x, y); (void)r; CHECK_EQ_I(errno, EDOM); }
TC_CASE(inf_x) { volatile float x = v_one / v_zero, y = 2.0f; volatile float r; errno = 0; r = fmodf(x, y); (void)r; CHECK_EQ_I(errno, EDOM); }

int main(void) { tc_begin(); TC_RUN(div_zero); TC_RUN(inf_x); return tc_end(); }

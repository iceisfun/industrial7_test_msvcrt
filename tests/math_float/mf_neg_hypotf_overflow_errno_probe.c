/* TC
 * id:    mf_neg_hypotf_overflow_errno_probe
 * area:  math_float
 * funcs: _hypotf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _hypot (ERANGE on overflow)
 * note:  PROBE: _hypotf(3e38f,3e38f) sets errno=ERANGE. Docs describe it for _hypot (double); the float variant may
 *        compute in double and narrow without setting errno.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
static volatile float big = 3e38f;
TC_CASE(errno_erange) { float r; errno = 0; r = _hypotf(big, big); CHECK(r > 3.4e38f); CHECK_EQ_I(errno, ERANGE); }
int main(void) { tc_begin(); TC_RUN(errno_erange); return tc_end(); }

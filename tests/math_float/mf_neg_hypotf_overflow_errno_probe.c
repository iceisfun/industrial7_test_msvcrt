/* TC
 * id:    mf_neg_hypotf_overflow_errno_probe
 * area:  math_float
 * funcs:
 * procs: _hypotf
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
/* F: a direct call of this name linked/called with the wrong signature on mingw (baseline results were 0 for every input); the DLL export called through GetProcAddress with the real float prototype returns the documented values */
TC_DECL_PROC(float, _hypotf, (float, float));
#undef _hypotf
#define _hypotf tc_p__hypotf
static volatile float big = 3e38f;
TC_CASE(errno_erange) { float r; errno = 0; r = _hypotf(big, big); CHECK(r > 3.4e38f); CHECK_EQ_I(errno, ERANGE); }
int main(void) { tc_begin(); TC_LOAD_PROC(_hypotf); if (!tc_p__hypotf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(errno_erange); return tc_end(); }

/* TC
 * id:    mf_hypotf_nooverflow_probe
 * area:  math_float
 * funcs:
 * procs: _hypotf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  observed
 * ref:   C11 7.12.7.3 hypot ("without undue overflow")
 * note:  PROBE: _hypotf(1e30f,1e30f) ~ 1.41421e30 (no overflow in x*x). Likely true since computed in double or scaled.
 */
#include "tc.h"
#include <math.h>
TC_DECL_PROC(float, _hypotf, (float, float));  /* explicit prototype: mingw math.h gives none for ucrtbase, so a bare call was compiled as int _hypotf(double,double) */
#define _hypotf tc_p__hypotf
static volatile float big = 1e30f;
TC_CASE(big) { float r = _hypotf(big, big); CHECK(r < 3e30f); CHECK_NEAR(r / 1e30f, 1.4142135f, 1e-5); }
int main(void) { tc_begin(); TC_LOAD_PROC(_hypotf); if (!tc_p__hypotf) { tc_write("TC FAIL " TC_TEST_ID "::proc not exported\n"); return 1; } TC_RUN(big); return tc_end(); }

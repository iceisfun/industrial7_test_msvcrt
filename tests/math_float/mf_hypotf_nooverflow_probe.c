/* TC
 * id:    mf_hypotf_nooverflow_probe
 * area:  math_float
 * funcs: _hypotf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.12.7.3 hypot ("without undue overflow")
 * note:  PROBE: _hypotf(1e30f,1e30f) ~ 1.41421e30 (no overflow in x*x). Likely true since computed in double or scaled.
 */
#include "tc.h"
#include <math.h>
static volatile float big = 1e30f;
TC_CASE(big) { float r = _hypotf(big, big); CHECK(r < 3e30f); CHECK_NEAR(r / 1e30f, 1.4142135f, 1e-5); }
int main(void) { tc_begin(); TC_RUN(big); return tc_end(); }

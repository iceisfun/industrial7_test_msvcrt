/* TC
 * id:    m99_chgsign_zero_probe
 * area:  math_c99
 * funcs: _chgsign
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _chgsign (does not state the zero case)
 * note:  PROBE: whether _chgsign(+0.0) returns -0.0 and _chgsign(-0.0) returns +0.0 (sign-bit flip, not 0-x).
 */
#include "tc.h"
#include <math.h>
#include <float.h>

TC_CASE(pos_zero) { volatile double z = 0.0; CHECK_BITS_D(_chgsign(z), -0.0); }
TC_CASE(neg_zero) { volatile double z = -0.0; CHECK_BITS_D(_chgsign(z), 0.0); }
int main(void) { tc_begin(); TC_RUN(pos_zero); TC_RUN(neg_zero); return tc_end(); }

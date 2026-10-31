/* TC
 * id:    m99_fmax_fmin_signed_zero
 * area:  math_c99
 * funcs: fmax fmin
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   C11 F.10.9.2 (sign of zero result is only recommended); MS docs fmax
 * note:  PROBE: whether Windows fmax(-0,+0) returns +0 and fmin(-0,+0) returns -0 in both argument orders.
 */
#include "tc.h"
#include <math.h>
#include <float.h>

TC_CASE(fmax_nz_pz) { volatile double p = 0.0, n = -0.0; CHECK_BITS_D(fmax(n, p), 0.0); }
TC_CASE(fmax_pz_nz) { volatile double p = 0.0, n = -0.0; CHECK_BITS_D(fmax(p, n), 0.0); }
TC_CASE(fmin_nz_pz) { volatile double p = 0.0, n = -0.0; CHECK_BITS_D(fmin(n, p), -0.0); }
TC_CASE(fmin_pz_nz) { volatile double p = 0.0, n = -0.0; CHECK_BITS_D(fmin(p, n), -0.0); }
int main(void) { tc_begin(); TC_RUN(fmax_nz_pz); TC_RUN(fmax_pz_nz); TC_RUN(fmin_nz_pz); TC_RUN(fmin_pz_nz); return tc_end(); }

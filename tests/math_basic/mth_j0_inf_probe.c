/* TC
 * id:    mth_j0_inf_probe
 * area:  math_basic
 * funcs: _j0
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  observed
 * ref:   C11 F.10 (j0 is POSIX; limit at infinity is 0)
 * note:  OBSERVED on Win10 22H2: _j0(+inf) returns a negative quiet NaN (0xfff8000000000000), not 0.
 *        PROBE: _j0(+inf) returns 0 (not NaN) on every CRT; legacy CRTs may report a domain error instead.
 */
#include "tc.h"
#include <math.h>

static volatile double v_zero = 0.0, v_one = 1.0;

TC_CASE(inf) { volatile double x = v_one / v_zero; CHECK_IS_NAN(_j0(x)); }

int main(void) { tc_begin(); TC_RUN(inf); return tc_end(); }

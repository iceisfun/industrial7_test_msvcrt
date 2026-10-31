/* TC
 * id:    mth_jn_parity_probe
 * area:  math_basic
 * funcs: _jn
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C99 jn / A-S 9.1.5
 * note:  Probe: _jn(n,-x) == (-1)^n * _jn(n,x) for n=2,3. MS docs do not state negative-x behaviour for _jn; math identity only.
 */
#include "tc.h"
#include <math.h>
#include <float.h>

TC_CASE(parity) { volatile double x=1.0, m=-1.0; CHECK_NEAR(_jn(2,m),_jn(2,x),1e-12); CHECK_NEAR(_jn(3,m),-_jn(3,x),1e-12); }

int main(void) { tc_begin(); TC_RUN(parity); return tc_end(); }

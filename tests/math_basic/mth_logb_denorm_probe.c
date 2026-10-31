/* TC
 * id:    mth_logb_denorm_probe
 * area:  math_basic
 * funcs: _logb
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.12.6.11 logb (subnormal -> true exponent)
 * note:  Probe: _logb(4.94e-324) == -1074 (true exponent). Older CRTs may report -1022 for subnormals.
 */
#include "tc.h"
#include <math.h>
#include <float.h>

static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_CASE(denorm_min) { volatile double d=mk(1ULL); CHECK_BITS_D(_logb(d),-1074.0); }

int main(void) { tc_begin(); TC_RUN(denorm_min); return tc_end(); }

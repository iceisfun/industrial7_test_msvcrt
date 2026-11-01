/* TC
 * id:    mth_logb_special
 * area:  math_basic
 * funcs: _logb
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   C11 F.10.3.11 logb
 * note:  OBSERVED on Win10 22H2: _logb(-INF) returns -INF (0xfff0...), deviating from C11 F.10.3.11 logb(+-inf)=+inf; _logb(+INF)=+INF.
 *        _logb(+-INF)=+INF and _logb(NaN)=NaN per MS docs; subnormal case split into mth_logb_denorm_probe
 */
#include "tc.h"
#include <math.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_CASE(inf) { volatile double i=v_one/v_zero; CHECK_BITS_D(_logb(i),i); CHECK_BITS_D(_logb(-i),-i); }
TC_CASE(nan) { volatile double n=mk(0x7FF8000000000000ULL); CHECK_IS_NAN(_logb(n)); }

int main(void) { tc_begin(); TC_RUN(inf); TC_RUN(nan); return tc_end(); }

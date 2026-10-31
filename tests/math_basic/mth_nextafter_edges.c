/* TC
 * id:    mth_nextafter_edges
 * area:  math_basic
 * funcs: _nextafter
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   MS docs _nextafter; C11 7.12.11.3
 */
#include "tc.h"
#include <math.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_CASE(overflow) { volatile double a=DBL_MAX,i=v_one/v_zero; CHECK_BITS_D(_nextafter(a,i),i); }
TC_CASE(from_inf) { volatile double i=v_one/v_zero,z=0.0; CHECK_BITS_D(_nextafter(i,z),DBL_MAX); CHECK_BITS_D(_nextafter(-i,z),-DBL_MAX); }
TC_CASE(into_subnormal) { volatile double a=DBL_MIN,z=0.0; CHECK_BITS_D(_nextafter(a,z),mk(0x000FFFFFFFFFFFFFULL)); }

int main(void) { tc_begin(); TC_RUN(overflow); TC_RUN(from_inf); TC_RUN(into_subnormal); return tc_end(); }

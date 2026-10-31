/* TC
 * id:    mth_nextafter_basic
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
TC_CASE(up) { volatile double a=1.0,b=2.0; CHECK_BITS_D(_nextafter(a,b),mk(0x3FF0000000000001ULL)); }
TC_CASE(down) { volatile double a=1.0,b=0.0; CHECK_BITS_D(_nextafter(a,b),mk(0x3FEFFFFFFFFFFFFFULL)); }
TC_CASE(equal) { volatile double a=1.0,b=1.0; CHECK_BITS_D(_nextafter(a,b),1.0); }
TC_CASE(from_zero) { volatile double a=0.0,b=1.0,c=-1.0; CHECK_BITS_D(_nextafter(a,b),mk(1ULL)); CHECK_BITS_D(_nextafter(a,c),mk(0x8000000000000001ULL)); }
TC_CASE(negative) { volatile double a=-1.0,b=-2.0; CHECK_BITS_D(_nextafter(a,b),mk(0xBFF0000000000001ULL)); }

int main(void) { tc_begin(); TC_RUN(up); TC_RUN(down); TC_RUN(equal); TC_RUN(from_zero); TC_RUN(negative); return tc_end(); }

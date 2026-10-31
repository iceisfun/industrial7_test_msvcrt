/* TC
 * id:    mth_copysign_basic
 * area:  math_basic
 * funcs: _copysign
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   MS docs _copysign; C11 7.12.11.1
 */
#include "tc.h"
#include <math.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_CASE(finite) { volatile double a=3.0,b=-3.0,p=1.0,m=-1.0; CHECK_BITS_D(_copysign(a,m),-3.0); CHECK_BITS_D(_copysign(b,p),3.0); }
TC_CASE(zeros) { volatile double z=0.0,nz=-0.0,p=1.0,m=-1.0; CHECK_BITS_D(_copysign(z,m),-0.0); CHECK_BITS_D(_copysign(nz,p),0.0); }
TC_CASE(inf) { volatile double i=v_one/v_zero,m=-1.0; CHECK_BITS_D(_copysign(i,m),-i); }
TC_CASE(nan) { volatile double n=mk(0x7FF8000000000000ULL),m=-1.0; CHECK((tc_dbits(_copysign(n,m))>>63)==1); }

int main(void) { tc_begin(); TC_RUN(finite); TC_RUN(zeros); TC_RUN(inf); TC_RUN(nan); return tc_end(); }

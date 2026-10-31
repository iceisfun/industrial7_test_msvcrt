/* TC
 * id:    mth_finite_basic
 * area:  math_basic
 * funcs: _finite
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   MS docs _finite
 */
#include "tc.h"
#include <math.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_CASE(zeros) { volatile double z=0.0,nz=-0.0; CHECK(_finite(z)!=0); CHECK(_finite(nz)!=0); }
TC_CASE(extremes) { volatile double a=DBL_MAX,b=mk(1ULL),c=-1.5; CHECK(_finite(a)!=0); CHECK(_finite(b)!=0); CHECK(_finite(c)!=0); }

int main(void) { tc_begin(); TC_RUN(zeros); TC_RUN(extremes); return tc_end(); }

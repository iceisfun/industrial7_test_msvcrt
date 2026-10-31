/* TC
 * id:    mth_chgsign_basic
 * area:  math_basic
 * funcs: _chgsign
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   MS docs _chgsign
 */
#include "tc.h"
#include <math.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_CASE(finite) { volatile double a=3.0,b=-3.0; CHECK_BITS_D(_chgsign(a),-3.0); CHECK_BITS_D(_chgsign(b),3.0); }
TC_CASE(zeros) { volatile double z=0.0,nz=-0.0; CHECK_BITS_D(_chgsign(z),-0.0); CHECK_BITS_D(_chgsign(nz),0.0); }
TC_CASE(inf) { volatile double i=v_one/v_zero; CHECK_BITS_D(_chgsign(i),-i); }
TC_CASE(nan) { volatile double n=mk(0x7FF8000000000000ULL); unsigned long long r=tc_dbits(_chgsign(n)); CHECK((r>>63)==1); CHECK_IS_NAN(_chgsign(n)); }

int main(void) { tc_begin(); TC_RUN(finite); TC_RUN(zeros); TC_RUN(inf); TC_RUN(nan); return tc_end(); }

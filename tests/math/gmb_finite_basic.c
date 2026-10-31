/* TC
 * id:    gmb_finite_basic
 * area:  math
 * funcs: _finite
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _finite, _finitef
 */
#include "tc.h"
#include <float.h>

static double from_bits(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }

TC_CASE(zeros_and_normals)
{
    volatile double z = 0.0, nz = -0.0, a = 1e308, m = DBL_MAX, nm = -DBL_MAX;
    CHECK(_finite(z) != 0); CHECK(_finite(nz) != 0); CHECK(_finite(a) != 0); CHECK(_finite(m) != 0); CHECK(_finite(nm) != 0);
}
TC_CASE(denormal) { volatile double d = 4.9406564584124654e-324; CHECK(_finite(d) != 0); }
TC_CASE(inf) { volatile double p = from_bits(0x7FF0000000000000ULL), n = from_bits(0xFFF0000000000000ULL); CHECK_EQ_I(_finite(p), 0); CHECK_EQ_I(_finite(n), 0); }
TC_CASE(nan)
{
    volatile double q = from_bits(0x7FF8000000000000ULL), nq = from_bits(0xFFF8000000000000ULL), s = from_bits(0x7FF0000000000001ULL);
    CHECK_EQ_I(_finite(q), 0); CHECK_EQ_I(_finite(nq), 0); CHECK_EQ_I(_finite(s), 0);
}
int main(void) { tc_begin(); TC_RUN(zeros_and_normals); TC_RUN(denormal); TC_RUN(inf); TC_RUN(nan); return tc_end(); }

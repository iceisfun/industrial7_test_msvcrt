/* TC
 * id:    mf_isnanf_basic
 * area:  math_float
 * funcs: _isnanf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _isnan, _isnanf (float.h)
 */
#include "tc.h"
#include <float.h>
static float mk(unsigned int b) { union { unsigned int u; float f; } x; x.u = b; return x.f; }
static volatile unsigned int b_qnan = 0x7FC00000u, b_snan = 0x7F800001u, b_mnan = 0xFFC00000u, b_inf = 0x7F800000u,
    b_minf = 0xFF800000u, b_zero = 0, b_one = 0x3F800000u, b_max = 0x7F7FFFFFu, b_den = 0x00000001u;

TC_CASE(nans) { CHECK(_isnanf(mk(b_qnan)) != 0); CHECK(_isnanf(mk(b_snan)) != 0); CHECK(_isnanf(mk(b_mnan)) != 0); }
TC_CASE(infs) { CHECK_EQ_I(_isnanf(mk(b_inf)), 0); CHECK_EQ_I(_isnanf(mk(b_minf)), 0); }
TC_CASE(non_nan) { CHECK_EQ_I(_isnanf(mk(b_zero)), 0); CHECK_EQ_I(_isnanf(mk(b_one)), 0); CHECK_EQ_I(_isnanf(mk(b_max)), 0); CHECK_EQ_I(_isnanf(mk(b_den)), 0); }
int main(void) { tc_begin(); TC_RUN(nans); TC_RUN(infs); TC_RUN(non_nan); return tc_end(); }

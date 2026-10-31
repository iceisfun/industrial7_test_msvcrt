/* TC
 * id:    mf_finitef_basic
 * area:  math_float
 * funcs: _finitef
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _finite, _finitef
 */
#include "tc.h"
#include <float.h>
#include <math.h>
static float mk(unsigned int b) { union { unsigned int u; float f; } x; x.u = b; return x.f; }
static volatile unsigned int b_zero = 0, b_one5 = 0x3FC00000u, b_den = 0x00400000u, b_max = 0x7F7FFFFFu,
    b_pinf = 0x7F800000u, b_ninf = 0xFF800000u, b_nan = 0x7FC00000u, b_nzero = 0x80000000u;

TC_CASE(finite_values) { CHECK(_finitef(mk(b_zero)) != 0); CHECK(_finitef(mk(b_nzero)) != 0); CHECK(_finitef(mk(b_one5)) != 0); CHECK(_finitef(mk(b_max)) != 0); }
TC_CASE(denormal) { CHECK(_finitef(mk(b_den)) != 0); CHECK(_finitef(mk(b_den | 0x80000000u)) != 0); }
TC_CASE(infinities) { CHECK_EQ_I(_finitef(mk(b_pinf)), 0); CHECK_EQ_I(_finitef(mk(b_ninf)), 0); }
TC_CASE(nan) { CHECK_EQ_I(_finitef(mk(b_nan)), 0); CHECK_EQ_I(_finitef(mk(b_nan | 0x80000000u)), 0); }
int main(void) { tc_begin(); TC_RUN(finite_values); TC_RUN(denormal); TC_RUN(infinities); TC_RUN(nan); return tc_end(); }

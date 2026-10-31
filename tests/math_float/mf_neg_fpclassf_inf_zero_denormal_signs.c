/* TC
 * id:    mf_neg_fpclassf_inf_zero_denormal_signs
 * area:  math_float
 * funcs: _fpclassf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _fpclass, _fpclassf (float.h _FPCLASS_*)
 * note:  float denormal classification moved to mf_neg_fpclassf_denormal_probe (old x64 CRTs may widen to double first).
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;
static float fb(unsigned int u) { union { float f; unsigned int u; } x; x.u = u; return x.f; }
#define SENT 1234

TC_CASE(negzero_nz) { volatile float x = -0.0f; CHECK_EQ_I(_fpclassf(x), _FPCLASS_NZ); }
TC_CASE(poszero_pz) { volatile float x = 0.0f; CHECK_EQ_I(_fpclassf(x), _FPCLASS_PZ); }
TC_CASE(ninf) { volatile float x = -v_inf; CHECK_EQ_I(_fpclassf(x), _FPCLASS_NINF); }
TC_CASE(pinf) { volatile float x = v_inf; CHECK_EQ_I(_fpclassf(x), _FPCLASS_PINF); }
TC_CASE(neg_normal_nn) { volatile float x = -1.5f; CHECK_EQ_I(_fpclassf(x), _FPCLASS_NN); }
TC_CASE(neg_qnan_qnan) { volatile float x = fb(0xffc00000u); CHECK_EQ_I(_fpclassf(x), _FPCLASS_QNAN); }

int main(void)
{
    tc_begin(); v_inf = v_one / v_zero;
    TC_RUN(negzero_nz); TC_RUN(poszero_pz);
    TC_RUN(ninf); TC_RUN(pinf); TC_RUN(neg_normal_nn); TC_RUN(neg_qnan_qnan);
    return tc_end();
}

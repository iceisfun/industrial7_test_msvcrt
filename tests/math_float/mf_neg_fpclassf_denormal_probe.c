/* TC
 * id:    mf_neg_fpclassf_denormal_probe
 * area:  math_float
 * funcs: _fpclassf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _fpclass, _fpclassf (float.h _FPCLASS_*)
 * note:  probes that _fpclassf classifies a FLOAT denormal as _FPCLASS_ND/_FPCLASS_PD. Documented for denormals,
 *        but pre-UCRT x64 implementations might widen to double first (where it would be a normal number).
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;
static float fb(unsigned int u) { union { float f; unsigned int u; } x; x.u = u; return x.f; }
#define SENT 1234

TC_CASE(neg_denormal_nd) { volatile float x = fb(0x80000001u); CHECK_EQ_I(_fpclassf(x), _FPCLASS_ND); }
TC_CASE(pos_denormal_pd) { volatile float x = fb(0x00000001u); CHECK_EQ_I(_fpclassf(x), _FPCLASS_PD); }

int main(void)
{
    tc_begin(); v_inf = v_one / v_zero;
    TC_RUN(neg_denormal_nd); TC_RUN(pos_denormal_pd);
    return tc_end();
}

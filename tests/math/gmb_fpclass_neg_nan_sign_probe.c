/* TC
 * id:    gmb_fpclass_neg_nan_sign_probe
 * area:  math
 * funcs: _fpclass
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _fpclass, _fpclassf (float.h _FPCLASS_* constants)
 * note:  PROBE: classification of NaNs of either sign, signalling NaNs, and boundary denormals.
 *        sNaN may be quieted on call/return by some implementations, so it has its own case.
 *        The "_neg_" id: inputs are "not a number / not normal".
 */
#include "tc.h"
#include <float.h>

#ifndef _FPCLASS_SNAN
#define _FPCLASS_SNAN 0x0001
#define _FPCLASS_QNAN 0x0002
#define _FPCLASS_NINF 0x0004
#define _FPCLASS_NN   0x0008
#define _FPCLASS_ND   0x0010
#define _FPCLASS_NZ   0x0020
#define _FPCLASS_PZ   0x0040
#define _FPCLASS_PD   0x0080
#define _FPCLASS_PN   0x0100
#define _FPCLASS_PINF 0x0200
#endif

static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static volatile unsigned long long v_bits;
static int cls(unsigned long long u) { v_bits = u; return _fpclass(mk(v_bits)); }

TC_CASE(neg_qnan) { CHECK_EQ_I(cls(0xFFF8000000000000ULL), _FPCLASS_QNAN); }
TC_CASE(pos_qnan_payload) { CHECK_EQ_I(cls(0x7FF8000000000001ULL), _FPCLASS_QNAN); }
TC_CASE(neg_snan) { CHECK_EQ_I(cls(0xFFF0000000000001ULL), _FPCLASS_SNAN); }
TC_CASE(pos_snan) { CHECK_EQ_I(cls(0x7FF0000000000001ULL), _FPCLASS_SNAN); }
TC_CASE(denormal_boundary)
{
    CHECK_EQ_I(cls(0x0010000000000000ULL), _FPCLASS_PN);  /* DBL_MIN */
    CHECK_EQ_I(cls(0x0008000000000000ULL), _FPCLASS_PD);  /* DBL_MIN/2 */
    CHECK_EQ_I(cls(0x000FFFFFFFFFFFFFULL), _FPCLASS_PD);
    CHECK_EQ_I(cls(0x8010000000000000ULL), _FPCLASS_NN);
    CHECK_EQ_I(cls(0x800FFFFFFFFFFFFFULL), _FPCLASS_ND);
    CHECK_EQ_I(cls(0x0000000000000001ULL), _FPCLASS_PD);
}

int main(void)
{
    tc_begin();
    TC_RUN(neg_qnan); TC_RUN(pos_qnan_payload); TC_RUN(neg_snan); TC_RUN(pos_snan); TC_RUN(denormal_boundary);
    return tc_end();
}

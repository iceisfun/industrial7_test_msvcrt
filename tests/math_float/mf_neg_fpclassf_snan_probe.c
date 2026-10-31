/* TC
 * id:    mf_neg_fpclassf_snan_probe
 * area:  math_float
 * funcs:
 * procs: _fpclassf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _fpclass, _fpclassf (_FPCLASS_SNAN 0x0001, _FPCLASS_QNAN 0x0002)
 * note:  PROBE: whether a signalling-NaN bit pattern passed as a float argument is still classified as
 *        _FPCLASS_SNAN (not silently quieted by argument passing / loads). The pattern 0x7F800001 is a
 *        signalling NaN by IEEE 754. _fpclassf is x64-only in some CRTs, so every case skips if missing.
 */
#include "tc.h"

typedef int (__cdecl *fpclassf_t)(float);
static fpclassf_t p_fpclassf;
#define FPC_SNAN 0x0001
#define FPC_QNAN 0x0002
#define FPC_NINF 0x0004
#define FPC_PINF 0x0200

static float from_bits(unsigned int u) { union { float f; unsigned int u; } x; x.u = u; return x.f; }
static volatile unsigned int v_snan_pos = 0x7F800001u, v_snan_neg = 0xFF800001u, v_qnan_pay = 0x7FC00001u, v_qnan_neg = 0xFFC00000u;

TC_CASE(snan_positive) { if (!p_fpclassf) TC_SKIP("_fpclassf missing"); CHECK_EQ_I(p_fpclassf(from_bits(v_snan_pos)), FPC_SNAN); }
TC_CASE(snan_negative_sign) { if (!p_fpclassf) TC_SKIP("_fpclassf missing"); CHECK_EQ_I(p_fpclassf(from_bits(v_snan_neg)), FPC_SNAN); }
TC_CASE(qnan_payload) { if (!p_fpclassf) TC_SKIP("_fpclassf missing"); CHECK_EQ_I(p_fpclassf(from_bits(v_qnan_pay)), FPC_QNAN); }
TC_CASE(qnan_negative) { if (!p_fpclassf) TC_SKIP("_fpclassf missing"); CHECK_EQ_I(p_fpclassf(from_bits(v_qnan_neg)), FPC_QNAN); }

int main(void)
{
    tc_begin(); p_fpclassf = (fpclassf_t)tc_crt_proc("_fpclassf");
    TC_RUN(snan_positive); TC_RUN(snan_negative_sign); TC_RUN(qnan_payload); TC_RUN(qnan_negative);
    return tc_end();
}

/* TC
 * id:    mf_float_special_roundtrip_bits
 * area:  math_float
 * funcs:
 * procs: sqrtf floorf _copysignf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.4.5 sqrt, F.10.6.2 floor, F.10.8.1 copysign; MS docs _copysign
 * note:  NaN payloads are NOT asserted; only NaN-ness and the sign bit that copysign defines.
 */
#include "tc.h"

typedef float (__cdecl *f1_t)(float);
typedef float (__cdecl *f2_t)(float, float);
static f1_t p_sqrtf, p_floorf; static f2_t p_copysignf;
static float from_bits(unsigned int u) { union { float f; unsigned int u; } x; x.u = u; return x.f; }
static volatile unsigned int v_qnan = 0x7FC00123u, v_nnan = 0xFFC00456u;
static volatile float v_pos = 1.0f, v_neg = -1.0f;

TC_CASE(sqrtf_payload_nan) { CHECK_IS_NAN(p_sqrtf(from_bits(v_qnan))); }
TC_CASE(floorf_neg_nan) { CHECK_IS_NAN(p_floorf(from_bits(v_nnan))); }
TC_CASE(copysign_nan_pos) { float r = p_copysignf(from_bits(v_nnan), v_pos); CHECK_IS_NAN(r); CHECK_EQ_U(tc_fbits(r) >> 31, 0); }
TC_CASE(copysign_nan_neg) { float r = p_copysignf(from_bits(v_qnan), v_neg); CHECK_IS_NAN(r); CHECK_EQ_U(tc_fbits(r) >> 31, 1); }
TC_CASE(copysign_from_nan_sign) { CHECK_BITS_F(p_copysignf(v_pos, from_bits(v_nnan)), -1.0f); CHECK_BITS_F(p_copysignf(v_neg, from_bits(v_qnan)), 1.0f); }

int main(void)
{
    tc_begin();
    p_sqrtf = (f1_t)tc_crt_proc("sqrtf"); p_floorf = (f1_t)tc_crt_proc("floorf"); p_copysignf = (f2_t)tc_crt_proc("_copysignf");
    if (!p_sqrtf || !p_floorf || !p_copysignf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; }
    TC_RUN(sqrtf_payload_nan); TC_RUN(floorf_neg_nan); TC_RUN(copysign_nan_pos); TC_RUN(copysign_nan_neg); TC_RUN(copysign_from_nan_sign);
    return tc_end();
}

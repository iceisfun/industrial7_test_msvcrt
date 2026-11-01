/* TC
 * id:    mf_neg_tanhf_atanf_errno_untouched
 * area:  math_float
 * funcs:
 * procs: tanhf atanf modff floorf ceilf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * note:  F: all of tanhf atanf modff floorf ceilf are called through the DLL export (GetProcAddress); the baseline linked libmingwex static copies. Expectations unchanged (probe: DLL floorf(nan)=0x7fc00000 e=0, ceilf(-inf)=0xff800000 e=0).
 * ref:   C11 F.10.2.6, F.10.1.3, F.10.6.1-2, F.10.8.2
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>
/* F: call the DLL export, never the libmingwex static copy (tc_crt_proc = GetProcAddress) */
TC_DECL_PROC(float, modff, (float, float *));
#undef modff
#define modff tc_p_modff
TC_DECL_PROC(float, floorf, (float));
TC_DECL_PROC(float, ceilf, (float));
#undef floorf
#define floorf tc_p_floorf
#undef ceilf
#define ceilf tc_p_ceilf

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;
static float fb(unsigned int u) { union { float f; unsigned int u; } x; x.u = u; return x.f; }
#define SENT 1234
TC_DECL_PROC(float, tanhf, (float));
#undef tanhf
#define tanhf tc_p_tanhf
TC_DECL_PROC(float, atanf, (float));
#undef atanf
#define atanf tc_p_atanf

TC_CASE(tanhf_inf_errno_unchanged) { volatile float x = v_inf, y = -v_inf; errno = SENT; CHECK_BITS_F(tanhf(x), 1.0f); CHECK_BITS_F(tanhf(y), -1.0f); CHECK_EQ_I(errno, SENT); }
TC_CASE(atanf_inf_errno_unchanged) { volatile float x = v_inf; errno = SENT; CHECK_NEAR(atanf(x), 1.57079632679489661923, 1e-6); CHECK_NEAR(atanf(-x), -1.57079632679489661923, 1e-6); CHECK_EQ_I(errno, SENT); }
TC_CASE(floorf_nan_errno_unchanged) { volatile float x = v_nan, r; errno = SENT; r = floorf(x); CHECK_IS_NAN(r); CHECK_EQ_I(errno, SENT); }
TC_CASE(ceilf_ninf_errno_unchanged) { volatile float x = -v_inf, r; errno = SENT; r = ceilf(x); CHECK(r == -v_inf); CHECK_EQ_I(errno, SENT); }
TC_CASE(modff_inf_frac_zero_signed)
{
    volatile float x = v_inf, y = -v_inf; float ip = 0.0f, f;
    errno = SENT;
    f = modff(x, &ip); CHECK_BITS_F(f, 0.0f); CHECK(ip == v_inf);
    f = modff(y, &ip); CHECK_BITS_F(f, -0.0f); CHECK(ip == -v_inf);
    CHECK_EQ_I(errno, SENT);
}
TC_CASE(iph_count_unchanged)
{
    volatile float x = v_inf, n = v_nan, r; float ip; int a;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    a = tc_iph_count;
    r = tanhf(x); r = atanf(x); r = floorf(n); r = ceilf(-x); r = modff(x, &ip); r = modff(n, &ip); (void)r;
    CHECK_EQ_I(tc_iph_count, a);
}

int main(void)
{
    tc_begin(); TC_LOAD_PROC(modff); if (!tc_p_modff) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_LOAD_PROC(tanhf); TC_LOAD_PROC(atanf); TC_LOAD_PROC(floorf); TC_LOAD_PROC(ceilf); if (!(tc_p_tanhf && tc_p_atanf && tc_p_floorf && tc_p_ceilf)) { tc_write("TC FAIL " TC_TEST_ID "::proc not exported\n"); return 1; }
    v_inf = v_one / v_zero; v_nan = fb(0x7fc00000);
    TC_RUN(tanhf_inf_errno_unchanged); TC_RUN(atanf_inf_errno_unchanged); TC_RUN(floorf_nan_errno_unchanged);
    TC_RUN(ceilf_ninf_errno_unchanged); TC_RUN(modff_inf_frac_zero_signed); TC_RUN(iph_count_unchanged);
    return tc_end();
}

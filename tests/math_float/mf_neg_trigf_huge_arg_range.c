/* TC
 * id:    mf_neg_trigf_huge_arg_range
 * area:  math_float
 * funcs:
 * procs: sinf cosf tanf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.1.5-7 (only inf is a domain error); MS docs sin, cos, tan
 * note:  ucrtbase only: pre-UCRT x64 sinf/cosf/tanf for |x| beyond 2^63 not known to return in-range values.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;
static float fb(unsigned int u) { union { float f; unsigned int u; } x; x.u = u; return x.f; }
#define SENT 1234
TC_DECL_PROC(float, sinf, (float));
#undef sinf
#define sinf tc_p_sinf
TC_DECL_PROC(float, cosf, (float));
#undef cosf
#define cosf tc_p_cosf
TC_DECL_PROC(float, tanf, (float));
#undef tanf
#define tanf tc_p_tanf

static int in_unit(float r) { return r >= -1.0f && r <= 1.0f; }
TC_CASE(sinf_fltmax_in_range) { volatile float x = FLT_MAX; CHECK(in_unit(sinf(x))); CHECK(in_unit(sinf(-x))); }
TC_CASE(cosf_fltmax_in_range) { volatile float x = FLT_MAX; CHECK(in_unit(cosf(x))); CHECK(in_unit(cosf(-x))); }
TC_CASE(tanf_fltmax_finite) { volatile float x = FLT_MAX, r = tanf(x); CHECK(r == r); CHECK(r != v_inf && r != -v_inf); }
TC_CASE(sinf_2pow100_in_range) { volatile float x = 1267650600228229401496703205376.0f; CHECK(in_unit(sinf(x))); CHECK(in_unit(cosf(x))); }
TC_CASE(errno_unchanged) { volatile float x = 1e20f, r; errno = SENT; r = sinf(x); r = cosf(x); r = tanf(x); (void)r; CHECK_EQ_I(errno, SENT); }
TC_CASE(no_iph)
{
    volatile float x = FLT_MAX, r; int a = tc_iph_count;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    a = tc_iph_count; r = sinf(x); r = cosf(x); r = tanf(x); (void)r;
    CHECK_EQ_I(tc_iph_count, a);
}

int main(void)
{
    tc_begin(); TC_LOAD_PROC(sinf); TC_LOAD_PROC(cosf); TC_LOAD_PROC(tanf); if (!(tc_p_sinf && tc_p_cosf && tc_p_tanf)) { tc_write("TC FAIL " TC_TEST_ID "::proc not exported\n"); return 1; }
    v_inf = v_one / v_zero;
    TC_RUN(sinf_fltmax_in_range); TC_RUN(cosf_fltmax_in_range); TC_RUN(tanf_fltmax_finite);
    TC_RUN(sinf_2pow100_in_range); TC_RUN(errno_unchanged); TC_RUN(no_iph);
    return tc_end();
}

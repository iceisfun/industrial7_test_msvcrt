/* TC
 * id:    gcy_complex_f_variants
 * area:  math_c99
 * funcs:
 * procs: cexpf clogf csinf ccosf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.3.7.1, 7.3.7.2, 7.3.5.4, 7.3.5.5, G.6.3.1, G.6.3.2; MS _Fcomplex = struct {float,float}, 8 bytes
 * note:  _Fcomplex passed/returned as a 64-bit value on x64. Declared as an 8-byte struct of two floats (same MS ABI class).
 */
#include "tc.h"
typedef struct { float re, im; } fcplx;
typedef fcplx (__cdecl *ffn_t)(fcplx);
static volatile float v_zero = 0.0f, v_one = 1.0f;
static fcplx F(float re, float im) { fcplx z; z.re = re; z.im = im; return z; }
static fcplx call(const char *name, float re, float im)
{
    ffn_t f = (ffn_t)tc_crt_proc(name); fcplx r = F(0, 0);
    if (!f) { tc_fail_expr(__FILE__, __LINE__, "export missing"); return r; }
    return f(F(re, im));
}
TC_CASE(cexpf_zero) { fcplx r = call("cexpf", 0.0f, 0.0f); CHECK_BITS_F(r.re, 1.0f); CHECK_BITS_F(r.im, 0.0f); }
TC_CASE(clogf_one) { fcplx r = call("clogf", 1.0f, 0.0f); CHECK_BITS_F(r.re, 0.0f); CHECK_BITS_F(r.im, 0.0f); }
TC_CASE(csinf_zero) { fcplx r = call("csinf", 0.0f, 0.0f); CHECK_BITS_F(r.re, 0.0f); CHECK_BITS_F(r.im, 0.0f); }
TC_CASE(ccosf_zero) { fcplx r = call("ccosf", 0.0f, 0.0f); CHECK_BITS_F(r.re, 1.0f); CHECK(r.im == 0.0f); }
TC_CASE(clogf_negzero) { fcplx r = call("clogf", -0.0f, 0.0f); CHECK(r.re == -(v_one / v_zero));
    CHECK_NEAR(r.im, 3.14159265358979, 1e-6); }
TC_CASE(cexpf_overflow) { fcplx r = call("cexpf", 100.0f, 0.0f); CHECK(r.re == v_one / v_zero); }

int main(void) { tc_begin(); TC_RUN(cexpf_zero); TC_RUN(clogf_one); TC_RUN(csinf_zero); TC_RUN(ccosf_zero); TC_RUN(clogf_negzero); TC_RUN(cexpf_overflow); return tc_end(); }

/* TC
 * id:    mf_frexpf_ldexpf_proc
 * area:  math_float
 * funcs:
 * procs: frexpf ldexpf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.12.6.4 frexp, 7.12.6.6 ldexp; MS docs frexp, ldexp
 * note:  frexpf/ldexpf may not be exported as such from the DLL; every case skips if the symbol is missing.
 */
#include "tc.h"
typedef float (__cdecl *frexpf_t)(float, int *);
typedef float (__cdecl *ldexpf_t)(float, int);
static frexpf_t p_frexpf; static ldexpf_t p_ldexpf;
static volatile float v_8 = 8.0f, v_0 = 0.0f, v_m3 = -3.0f, v_1 = 1.0f;
static volatile int v_10 = 10, v_m1 = -1, v_100 = 100;

TC_CASE(frexp_8) { int e = -99; float r; if (!p_frexpf) TC_SKIP("frexpf not exported"); r = p_frexpf(v_8, &e); CHECK_BITS_F(r, 0.5f); CHECK_EQ_I(e, 4); }
TC_CASE(frexp_0) { int e = -99; float r; if (!p_frexpf) TC_SKIP("frexpf not exported"); r = p_frexpf(v_0, &e); CHECK_BITS_F(r, 0.0f); CHECK_EQ_I(e, 0); }
TC_CASE(frexp_neg) { int e = -99; float r; if (!p_frexpf) TC_SKIP("frexpf not exported"); r = p_frexpf(v_m3, &e); CHECK_BITS_F(r, -0.75f); CHECK_EQ_I(e, 2); }
TC_CASE(ldexp_basic)
{
    if (!p_ldexpf) TC_SKIP("ldexpf not exported");
    CHECK_BITS_F(p_ldexpf(v_1, v_10), 1024.0f);
    CHECK_BITS_F(p_ldexpf(v_1, v_m1), 0.5f);
    CHECK_BITS_F(p_ldexpf(v_0, v_100), 0.0f);
}
int main(void)
{
    tc_begin();
    p_frexpf = (frexpf_t)tc_crt_proc("frexpf"); p_ldexpf = (ldexpf_t)tc_crt_proc("ldexpf");
    TC_RUN(frexp_8); TC_RUN(frexp_0); TC_RUN(frexp_neg); TC_RUN(ldexp_basic);
    return tc_end();
}

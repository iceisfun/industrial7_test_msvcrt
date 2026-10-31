/* TC
 * id:    m99_neg_iph_not_called_domain
 * area:  math_c99
 * procs: lgamma
 * funcs: cbrt log1p log2 expm1 tgamma erf fma lround llround nan fdim exp2 log2f tgammaf expm1f
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.1 (domain/range errors are not invalid parameters); MS docs math error handling
 * note:  Math domain/range errors must NOT invoke the invalid-parameter handler. Child process repeats the calls with the default (fatal) handler.
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>
#include <limits.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
static double d_inf(void) { return v_one / v_zero; }
static double d_nan(void) { return d_inf() - d_inf(); }
static float f_inf(void) { return f_one / f_zero; }
static float f_nan(void) { return f_inf() - f_inf(); }
static double d_snan(void) { union { unsigned long long u; double d; } x; x.u = 0x7ff0000000000001ULL; return x.d; }


TC_DECL_PROC(double, lgamma, (double));
static void calls(void)
{
    if (!tc_p_lgamma) ExitProcess(78);
    volatile double m1 = -1.0, m2 = -2.0, z = 0.0, e6 = 1e6, e3 = 1e3, big = 1e30, o = 1.0;
    volatile double r; volatile float fm1 = -1.0f, f100 = 100.0f; volatile float fr; volatile long lr; volatile long long llr;
    r = log2(m1); r = log1p(m2); r = tgamma(m1); r = tgamma(z); r = exp2(e6); r = expm1(e3);
    r = fma(z, d_inf(), o); lr = lround(d_nan()); llr = llround(big); r = tc_p_lgamma(z); r = cbrt(m2); r = erf(o); r = nan(""); r = fdim(o, z);
    fr = log2f(fm1); fr = tgammaf(fm1); fr = expm1f(f100);
    (void)r; (void)fr; (void)lr; (void)llr;
}
TC_CASE(handler_not_called)
{
    volatile double m1 = -1.0, m2 = -2.0, z = 0.0, e6 = 1e6, e3 = 1e3, big = 1e30, o = 1.0; volatile double r;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0;
    r = log2(m1);        CHECK_EQ_I(tc_iph_count, 0);
    r = log1p(m2);       CHECK_EQ_I(tc_iph_count, 0);
    r = tgamma(m1);      CHECK_EQ_I(tc_iph_count, 0);
    r = tgamma(z);       CHECK_EQ_I(tc_iph_count, 0);
    r = exp2(e6);        CHECK_EQ_I(tc_iph_count, 0);
    r = expm1(e3);       CHECK_EQ_I(tc_iph_count, 0);
    r = fma(z, d_inf(), o); CHECK_EQ_I(tc_iph_count, 0);
    (void)lround(d_nan()); CHECK_EQ_I(tc_iph_count, 0);
    (void)llround(big);  CHECK_EQ_I(tc_iph_count, 0);
    if (tc_p_lgamma) r = tc_p_lgamma(z);
    CHECK_EQ_I(tc_iph_count, 0);
    r = cbrt(m2); r = erf(o); r = nan(""); r = fdim(o, z);
    CHECK_EQ_I(tc_iph_count, 0);
    (void)r;
}
TC_CASE(float_handler_not_called)
{
    volatile float fm1 = -1.0f, f100 = 100.0f; volatile float r;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0;
    r = log2f(fm1);   CHECK_EQ_I(tc_iph_count, 0);
    r = tgammaf(fm1); CHECK_EQ_I(tc_iph_count, 0);
    r = expm1f(f100); CHECK_EQ_I(tc_iph_count, 0);
    (void)r;
}
TC_CASE(survives_default_handler)
{
    DWORD code = 0;
    CHECK(tc_spawn_self("calls", &code, 20000));
    CHECK_EQ_U(code, 77);
}
int main(void)
{
    const char *c;
    tc_begin(); TC_LOAD_PROC(lgamma);
    if ((c = tc_child_name())) { calls(); return 77; }
    TC_RUN(handler_not_called); TC_RUN(float_handler_not_called); TC_RUN(survives_default_handler);
    return tc_end();
}

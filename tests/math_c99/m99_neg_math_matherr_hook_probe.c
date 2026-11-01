/* TC
 * id:    m99_neg_math_matherr_hook_probe
 * area:  math_c99
 * funcs: log2 tgamma cbrt log1p
 * procs: __setusermatherr _set_matherr log
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _matherr, __setusermatherr
 * note:  A: observed on Win10 19045 ucrtbase: tgamma(-1), log1p(-2), cbrt(-1) do not call the __setusermatherr hook, but log2(-1) DOES (hook_calls == 1), as do sqrt/log/pow/asin/acos/fmod/remainder/sinh. log2 split into its own case. Was PROBE: C99 math functions (log2, tgamma, log1p) are assumed NOT to call the user matherr handler on domain errors. The legacy log(-1) control and the presence of _set_matherr / __setusermatherr are only PRINTED (TC INFO), not asserted.
 */
#include "tc.h"
#include <math.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static int hook_calls;
static int __cdecl hook(void *exc) { (void)exc; hook_calls++; return 0; }
typedef void (__cdecl *setm_t)(int (__cdecl *)(void *));
static setm_t p_setusermatherr, p_set_matherr;
static double (__cdecl *p_log)(double);

static void info(const char *what, int n)
{ char b[24]; tc_i64(n, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write("="); tc_write(b); tc_write("\n"); }

TC_CASE(presence) { info("__setusermatherr_exported", p_setusermatherr != NULL); info("_set_matherr_exported", p_set_matherr != NULL); }
TC_CASE(c99_not_hooked)
{
    volatile double a = -1.0, b = -2.0; double r;
    if (!p_setusermatherr) TC_SKIP("no __setusermatherr");
    p_setusermatherr(hook); hook_calls = 0;
    r = tgamma(a); CHECK(r != r);
    r = log1p(b); CHECK(r != r);
    r = cbrt(a); CHECK_NEAR(r, -1.0, 1e-15);
    CHECK_EQ_I(hook_calls, 0);
    p_setusermatherr(NULL);
}
TC_CASE(log2_is_hooked)
{
    volatile double a = -1.0; double r;
    if (!p_setusermatherr) TC_SKIP("no __setusermatherr");
    p_setusermatherr(hook); hook_calls = 0;
    r = log2(a); CHECK(r != r);
    CHECK_EQ_I(hook_calls, 1);   /* observed: log2 DOES dispatch to the user matherr hook */
    p_setusermatherr(NULL);
}
TC_CASE(legacy_control)
{
    volatile double a = -1.0; double r;
    if (!p_setusermatherr) TC_SKIP("no __setusermatherr");
    p_setusermatherr(hook); hook_calls = 0;
    if (!p_log) TC_SKIP("no log");
    r = p_log(a); CHECK(r != r);
    info("legacy_log_hook_calls", hook_calls);
    p_setusermatherr(NULL);
}
int main(void)
{
    tc_begin();
    p_setusermatherr = (setm_t)tc_crt_proc("__setusermatherr"); p_set_matherr = (setm_t)tc_crt_proc("_set_matherr");
    p_log = (double (__cdecl *)(double))tc_crt_proc("log");
    TC_RUN(presence); TC_RUN(c99_not_hooked); TC_RUN(log2_is_hooked); TC_RUN(legacy_control);
    return tc_end();
}

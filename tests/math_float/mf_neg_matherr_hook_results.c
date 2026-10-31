/* TC
 * id:    mf_neg_matherr_hook_results
 * area:  math_float
 * funcs: __setusermatherr
 * procs: sqrtf logf powf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   C11 F.10.4.5 sqrt, F.10.3.7 log, F.10.4.4 pow; MS docs sqrt, log, pow (-1 -> IND, 0 -> -INF, 0^neg -> INF)
 * note:  A returning (retval-untouched, returns 0) user hook must not change the IEEE results of
 *        sqrtf(-1)=NaN, logf(0)=-inf, powf(0,-1)=+inf. Whether the hook is called is probed elsewhere.
 */
#include "tc.h"

struct tc_exception { int type; char *name; double arg1, arg2, retval; };
typedef int (__cdecl *hook_t)(struct tc_exception *);
typedef void (__cdecl *setmatherr_t)(hook_t);
typedef float (__cdecl *f1_t)(float);
typedef float (__cdecl *f2_t)(float, float);
static setmatherr_t p_set; static f1_t p_sqrtf, p_logf; static f2_t p_powf;
static int __cdecl hook(struct tc_exception *e) { (void)e; return 0; }
static volatile float v_m1 = -1.0f, v_zero = 0.0f, v_one = 1.0f, v_inf;

TC_CASE(sqrtf_nan) { p_set(hook); CHECK_IS_NAN(p_sqrtf(v_m1)); p_set(NULL); }
TC_CASE(logf_neg_inf) { p_set(hook); CHECK_BITS_F(p_logf(v_zero), -v_inf); p_set(NULL); }
TC_CASE(powf_pos_inf) { p_set(hook); CHECK_BITS_F(p_powf(v_zero, v_m1), v_inf); p_set(NULL); }

int main(void)
{
    tc_begin();
    p_set = (setmatherr_t)tc_crt_proc("__setusermatherr");
    p_sqrtf = (f1_t)tc_crt_proc("sqrtf"); p_logf = (f1_t)tc_crt_proc("logf"); p_powf = (f2_t)tc_crt_proc("powf");
    if (!p_set || !p_sqrtf || !p_logf || !p_powf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; }
    v_inf = v_one / v_zero;
    TC_RUN(sqrtf_nan); TC_RUN(logf_neg_inf); TC_RUN(powf_pos_inf);
    return tc_end();
}

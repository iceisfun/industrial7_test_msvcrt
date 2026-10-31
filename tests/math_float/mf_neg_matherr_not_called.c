/* TC
 * id:    mf_neg_matherr_not_called
 * area:  math_float
 * funcs: __setusermatherr
 * procs: sqrtf logf powf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _matherr ("not supported by the UCRT"), __setusermatherr
 * note:  PROBE: the UCRT documents that a user _matherr hook is no longer called. Asserts the hook is NOT
 *        invoked for float domain/pole errors (count == 0) and logs the count. Result values with the hook
 *        installed are checked in mf_neg_matherr_hook_results (kept separate on purpose).
 */
#include "tc.h"

struct tc_exception { int type; char *name; double arg1, arg2, retval; };
typedef int (__cdecl *hook_t)(struct tc_exception *);
typedef void (__cdecl *setmatherr_t)(hook_t);
typedef float (__cdecl *f1_t)(float);
typedef float (__cdecl *f2_t)(float, float);
static setmatherr_t p_set; static f1_t p_sqrtf, p_logf; static f2_t p_powf;
static int hook_calls;
static int __cdecl hook(struct tc_exception *e) { (void)e; hook_calls++; return 0; }
static volatile float v_m1 = -1.0f, v_zero = 0.0f, v_f; 

TC_CASE(sqrtf_neg) { hook_calls = 0; p_set(hook); v_f = p_sqrtf(v_m1); p_set(NULL); CHECK_EQ_I(hook_calls, 0); }
TC_CASE(logf_zero) { hook_calls = 0; p_set(hook); v_f = p_logf(v_zero); p_set(NULL); CHECK_EQ_I(hook_calls, 0); }
TC_CASE(logf_neg)  { hook_calls = 0; p_set(hook); v_f = p_logf(v_m1); p_set(NULL); CHECK_EQ_I(hook_calls, 0); }
TC_CASE(powf_zero_neg) { hook_calls = 0; p_set(hook); v_f = p_powf(v_zero, v_m1); p_set(NULL); CHECK_EQ_I(hook_calls, 0); }

int main(void)
{
    tc_begin();
    p_set = (setmatherr_t)tc_crt_proc("__setusermatherr");
    p_sqrtf = (f1_t)tc_crt_proc("sqrtf"); p_logf = (f1_t)tc_crt_proc("logf"); p_powf = (f2_t)tc_crt_proc("powf");
    if (!p_set || !p_sqrtf || !p_logf || !p_powf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; }
    TC_RUN(sqrtf_neg); TC_RUN(logf_zero); TC_RUN(logf_neg); TC_RUN(powf_zero_neg);
    return tc_end();
}

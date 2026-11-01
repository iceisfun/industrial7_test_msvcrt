/* TC
 * id:    mth_neg_matherr_ucrt_dispatched_probe
 * area:  math_basic
 * funcs:
 * procs: __setusermatherr sqrt log
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _matherr / __setusermatherr (UCRT: user math error handler is not supported)
 * note:  PROBE: MS docs for _matherr (msvc-140+) say the math library functions call _matherr on error, so a handler Observed on an FMA3/AVX2 CPU (ucrtbase dispatches FMA3 paths); non-FMA path unverified.
 *        registered with __setusermatherr is called once for sqrt(-1) (type _DOMAIN=1, name "sqrt") and log(0) (type
 *        _SING=2, name "log"). Unsure the UCRT x64 sqrt/log still dispatch. errno is deliberately not asserted here.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <string.h>
/* F: call the DLL export, never the libmingwex static copy (tc_crt_proc = GetProcAddress) */
TC_DECL_PROC(double, sqrt, (double));
#undef sqrt
#define sqrt tc_p_sqrt
TC_DECL_PROC(double, log, (double));
#undef log
#define log tc_p_log

struct tc_exception { int type; char *name; double arg1, arg2, retval; };
typedef int (__cdecl *hook_t)(struct tc_exception *);
typedef void (__cdecl *setmatherr_t)(hook_t);
static setmatherr_t p_set;
static int h_calls, h_type, h_ret; static char h_name[32]; static double h_a1, h_a2, h_setretval; static int h_setret_on;
static int __cdecl hook(struct tc_exception *e)
{
    size_t i = 0;
    h_calls++; h_type = e->type; h_a1 = e->arg1; h_a2 = e->arg2;
    if (e->name) { for (; e->name[i] && i < 31; i++) h_name[i] = e->name[i]; }
    h_name[i] = 0;
    if (h_setret_on) e->retval = h_setretval;
    return h_ret;
}
static void reset(void) { h_calls = 0; h_type = -1; h_name[0] = 0; h_a1 = h_a2 = 0; h_ret = 0; h_setret_on = 0; }
static volatile double v_zero = 0.0, v_one = 1.0, v_m1 = -1.0, v_r;

TC_CASE(sqrt_neg)
{
    reset(); errno = 0; p_set(hook); v_r = sqrt(v_m1); p_set(NULL);
    CHECK_EQ_I(h_calls, 1); CHECK_EQ_I(h_type, 1); CHECK_STR(h_name, "sqrt");
}
TC_CASE(log_zero)
{
    reset(); errno = 0; p_set(hook); v_r = log(v_zero); p_set(NULL);
    CHECK_EQ_I(h_calls, 1); CHECK_EQ_I(h_type, 2); CHECK_STR(h_name, "log");
}

int main(void)
{
    tc_begin(); TC_LOAD_PROC(sqrt); TC_LOAD_PROC(log); if (!tc_p_sqrt || !tc_p_log) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; }
    p_set = (setmatherr_t)tc_crt_proc("__setusermatherr");
    if (!p_set) { tc_write("TC FAIL " TC_TEST_ID "::__setusermatherr missing\n"); return 1; }
    TC_RUN(sqrt_neg); TC_RUN(log_zero);
    return tc_end();
}

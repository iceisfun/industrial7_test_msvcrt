/* TC
 * id:    mth_neg_matherr_sqrt_domain_probe
 * area:  math_basic
 * funcs: sqrt
 * procs: __setusermatherr
 * crt:   msvcrt msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _matherr, __setusermatherr (pre-UCRT)
 * note:  PROBE: a handler registered with __setusermatherr is called for sqrt(-1) with type _DOMAIN(1), name "sqrt",
 *        arg1 -1; handler returns 0 so errno is set to EDOM. Unsure whether msvcr120 still dispatches.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <string.h>

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

TC_CASE(handler_called)
{
    reset(); errno = 0; p_set(hook); v_r = sqrt(v_m1); p_set(NULL);
    CHECK_EQ_I(h_calls, 1); CHECK_EQ_I(h_type, 1); CHECK_STR(h_name, "sqrt"); CHECK(h_a1 == -1.0);
}
TC_CASE(errno_set)
{
    reset(); errno = 0; p_set(hook); v_r = sqrt(v_m1); p_set(NULL);
    CHECK_EQ_I(errno, EDOM);
}
TC_CASE(restored)
{
    reset(); p_set(hook); p_set(NULL); v_r = sqrt(v_m1);
    CHECK_EQ_I(h_calls, 0);
}

int main(void)
{
    tc_begin();
    p_set = (setmatherr_t)tc_crt_proc("__setusermatherr");
    if (!p_set) { tc_write("TC FAIL " TC_TEST_ID "::__setusermatherr missing\n"); return 1; }
    TC_RUN(handler_called); TC_RUN(errno_set); TC_RUN(restored);
    return tc_end();
}

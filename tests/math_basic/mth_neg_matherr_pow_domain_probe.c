/* TC
 * id:    mth_neg_matherr_pow_domain_probe
 * area:  math_basic
 * funcs:
 * procs: __setusermatherr pow
 * crt:   msvcrt msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _matherr, pow
 * note:  PROBE: pow(-8,1/3) reports _DOMAIN(1) with name "pow" and both args forwarded; pow(0,-1) reports
 *        _SING(2) (accepts 1 or 2, type is logged by failure text). Handler called exactly once each.
 */
#include "tc.h"
#include <math.h>
typedef double (__cdecl *d2_t)(double, double);
static d2_t p_pow;
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

TC_CASE(neg_base_frac_exp)
{
    volatile double b = -8.0, e = 1.0 / 3.0;
    reset(); p_set(hook); v_r = p_pow(b, e); p_set(NULL);
    CHECK_EQ_I(h_calls, 1); CHECK_EQ_I(h_type, 1); CHECK_STR(h_name, "pow");
    CHECK(h_a1 == -8.0); CHECK(h_a2 == e);
}
TC_CASE(zero_neg_exp)
{
    reset(); p_set(hook); v_r = p_pow(v_zero, v_m1); p_set(NULL);
    CHECK_EQ_I(h_calls, 1); CHECK(h_type == 2 || h_type == 1); CHECK_STR(h_name, "pow");
}

int main(void)
{
    tc_begin();
    p_set = (setmatherr_t)tc_crt_proc("__setusermatherr");
    if (!p_set) { tc_write("TC FAIL " TC_TEST_ID "::__setusermatherr missing\n"); return 1; }
    p_pow = (d2_t)tc_crt_proc("pow"); if (!p_pow) { tc_write("TC FAIL " TC_TEST_ID "::pow missing\n"); return 1; }
    TC_RUN(neg_base_frac_exp); TC_RUN(zero_neg_exp);
    return tc_end();
}

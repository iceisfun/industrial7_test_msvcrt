/* TC
 * id:    mf_neg_matherr_msvcr100_probe
 * area:  math_float
 * funcs: __setusermatherr
 * procs: sqrtf logf
 * crt:   msvcr120 msvcr110 msvcr100
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _matherr, __setusermatherr (_DOMAIN 1, _SING 2)
 * note:  PROBE: on pre-UCRT CRTs, is a user _matherr hook called for float sqrtf(-1)/logf(0)? The call count and the
 *        reported function name are logged (TC INFO lines). Assertions are conditional: IF the hook is called, the
 *        error type must be _DOMAIN for sqrtf(-1) and _SING for logf(0). The IEEE results hold either way.
 */
#include "tc.h"

struct tc_exception { int type; char *name; double arg1, arg2, retval; };
typedef int (__cdecl *hook_t)(struct tc_exception *);
typedef void (__cdecl *setmatherr_t)(hook_t);
typedef float (__cdecl *f1_t)(float);
static setmatherr_t p_set; static f1_t p_sqrtf, p_logf;
static int hook_calls, last_type; static char last_name[32];
static int __cdecl hook(struct tc_exception *e)
{
    int i = 0; hook_calls++; last_type = e->type; last_name[0] = 0;
    if (e->name) for (; e->name[i] && i < 31; i++) last_name[i] = e->name[i];
    last_name[i] = 0;
    return 0;
}
static volatile float v_m1 = -1.0f, v_zero = 0.0f, v_f;
static float v_f_zero(void) { return v_zero; }

static void info(const char *fn)
{
    char b[24]; tc_write("TC INFO " TC_TEST_ID " "); tc_write(fn); tc_write(" calls="); tc_i64(hook_calls, b); tc_write(b);
    tc_write(" type="); tc_i64(last_type, b); tc_write(b); tc_write(" name="); tc_write(last_name); tc_write("\n");
}
TC_CASE(sqrtf_neg)
{
    hook_calls = 0; last_type = 0; p_set(hook); v_f = p_sqrtf(v_m1); p_set(NULL); info("sqrtf(-1)");
    CHECK_IS_NAN(v_f);
    if (hook_calls) CHECK_EQ_I(last_type, 1);
}
TC_CASE(logf_zero)
{
    hook_calls = 0; last_type = 0; p_set(hook); v_f = p_logf(v_zero); p_set(NULL); info("logf(0)");
    CHECK_BITS_F(v_f, -1.0f / v_f_zero());
    if (hook_calls) CHECK_EQ_I(last_type, 2);
}

int main(void)
{
    tc_begin();
    p_set = (setmatherr_t)tc_crt_proc("__setusermatherr");
    p_sqrtf = (f1_t)tc_crt_proc("sqrtf"); p_logf = (f1_t)tc_crt_proc("logf");
    if (!p_set || !p_sqrtf || !p_logf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; }
    TC_RUN(sqrtf_neg); TC_RUN(logf_zero);
    return tc_end();
}

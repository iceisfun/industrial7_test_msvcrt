/* TC
 * id:    mf_neg_fp_exception_unmasked_probe
 * area:  math_float
 * funcs:
 * procs: sqrtf logf _controlfp_s
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _controlfp_s, _EM_INVALID
 * note:  PROBE: with _EM_INVALID unmasked, what happens on sqrtf(-1) / logf(-1)? Child exit code is logged: 77 = returned normally (CRT masks/handles internally), 0xC000008E/0xC0000090/0xC0000091 = FP exception raised. Parent asserts only that the child terminated within the timeout with one of those documented outcomes.
 */
#include "tc.h"
TC_DECL_PROC(float, sqrtf, (float)); TC_DECL_PROC(float, logf, (float));
TC_DECL_PROC(int, _controlfp_s, (unsigned int *, unsigned int, unsigned int));
static volatile float v_m1 = -1.0f;

static int child_sqrt(void)
{
    unsigned int cur = 0; volatile float r;
    TC_LOAD_PROC(sqrtf); TC_LOAD_PROC(_controlfp_s);
    if (!tc_p_sqrtf || !tc_p__controlfp_s) return 78;
    tc_p__controlfp_s(&cur, 0, 0x10 /*_EM_INVALID*/);   /* mask bit cleared => unmasked */
    r = tc_p_sqrtf(v_m1); (void)r;
    return 77;
}
static int child_log(void)
{
    unsigned int cur = 0; volatile float r;
    TC_LOAD_PROC(logf); TC_LOAD_PROC(_controlfp_s);
    if (!tc_p_logf || !tc_p__controlfp_s) return 78;
    tc_p__controlfp_s(&cur, 0, 0x10);
    r = tc_p_logf(v_m1); (void)r;
    return 77;
}
static void check_code(const char *what, DWORD code)
{
    char b[24]; tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write(" exit=");
    tc_hex(code, b); tc_write(b); tc_write("\n");
    CHECK(code == 77 || code == 0xC000008Eu || code == 0xC0000090u || code == 0xC0000091u || code == 0xC0000092u || code == 0xC0000093u);
}
TC_CASE(sqrt_unmasked) { DWORD c = 0; CHECK(tc_spawn_self("sqrt", &c, 20000)); check_code("sqrtf", c); }
TC_CASE(log_unmasked)  { DWORD c = 0; CHECK(tc_spawn_self("log", &c, 20000)); check_code("logf", c); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return c[0] == 's' ? child_sqrt() : child_log();
    TC_RUN(sqrt_unmasked); TC_RUN(log_unmasked);
    return tc_end();
}

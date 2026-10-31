/* TC
 * id:    mf_neg_frexpf_null_exp_probe
 * area:  math_float
 * funcs:
 * procs: frexpf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs frexp (exp pointer not documented as validated)
 * note:  PROBE: frexpf(x, NULL) in a child process. Either the call is tolerated (child survives, exit code 77/100) or it
 *       crashes with an access violation (0xC0000005). The invalid-parameter handler must NOT fire; a fastfail exit
 *       (0xC0000409) or any other status is a failure.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>
#include <limits.h>

static volatile float v_zero = 0.0f, v_one = 1.0f;
static float F_INF(void) { return v_one / v_zero; }
static float F_NAN(void) { return v_zero / v_zero; }
#define SENT 0x5a5
#define NEED(p) do { if (!(p)) TC_SKIP("function not exported"); } while (0)

TC_DECL_PROC(float, frexpf, (float, int *));
static volatile int *v_nullexp;

static int child_main(const char *n)
{
    volatile float x = 8.0f, z = 0.0f; volatile float r;
    if (!tc_p_frexpf) return 3;
    tc_iph_install(); tc_iph_count = 0;
    if (n[0] == 'v') { r = tc_p_frexpf(x, (int *)v_nullexp); (void)r; return tc_iph_count ? 55 : 77; }
    if (n[0] == 'z') { r = tc_p_frexpf(z, (int *)v_nullexp); (void)r; return tc_iph_count ? 55 : 77; }
    r = tc_p_frexpf(x, (int *)v_nullexp); (void)r;
    return 100 + tc_iph_count;
}
static void run_child(const char *name, DWORD ok_code)
{
    DWORD code = 0;
    NEED(tc_p_frexpf);
    CHECK(tc_spawn_self(name, &code, 20000));
    CHECK(code == ok_code || code == TC_STATUS_ACCESS_VIOLATION);
}
TC_CASE(null_exp_child_exit_code) { run_child("v", 77); }
TC_CASE(null_exp_zero_value_child) { run_child("z", 77); }
TC_CASE(null_exp_iph_count_unchanged) { run_child("i", 100); }
int main(void)
{
    const char *c;
    tc_begin(); TC_LOAD_PROC(frexpf);
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(null_exp_child_exit_code); TC_RUN(null_exp_zero_value_child); TC_RUN(null_exp_iph_count_unchanged);
    return tc_end();
}

/* TC
 * id:    mth_neg_child_unmasked_overflow_probe
 * area:  math_basic
 * funcs:
 * procs: exp log _controlfp_s
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _controlfp_s (_EM_OVERFLOW 0x04, _EM_ZERODIVIDE 0x08: clearing the mask bit enables the exception)
 * note:  PROBE: a child that unmasks overflow (or zero-divide) then calls exp(1000) (log(0)). Either it dies with an exception
 *        (exit code not 0 and not 77) or the CRT returns inf and the child exits 0. Child exits 77 only if the call returned
 *        a non-inf value. Parent asserts code != 77 and records the code. Control child with default masks must exit 0.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0, v_nan_src;
static double pinf(void) { return v_one / v_zero; }
static double qnan(void) { return v_zero / v_zero; }
static int sgn(double d) { return (int)(tc_dbits(d) >> 63); }
static void info_i(const char *what, long long v) { char b[32]; tc_i64(v, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write("="); tc_write(b); tc_write("\n"); }

typedef int (__cdecl *cfs_t)(unsigned int *, unsigned int, unsigned int);
typedef double (__cdecl *d1_t)(double);
static volatile double v_r;

static void rec(const char *what, DWORD code) { char b[32]; tc_hex(code, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write("="); tc_write(b); tc_write("\n"); }
TC_CASE(overflow_unmasked) { DWORD code = 0; int ran = tc_spawn_self("ov", &code, 20000); CHECK(ran); rec("exp_unmasked_exit", code); CHECK(code != 77); }
TC_CASE(zerodiv_unmasked) { DWORD code = 0; int ran = tc_spawn_self("zd", &code, 20000); CHECK(ran); rec("log0_unmasked_exit", code); CHECK(code != 77); }
TC_CASE(control_masked) { DWORD code = 0; int ran = tc_spawn_self("ctl", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 0); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        cfs_t cfs = (cfs_t)tc_crt_proc("_controlfp_s");
        d1_t pe = (d1_t)tc_crt_proc("exp"), pl = (d1_t)tc_crt_proc("log");
        unsigned int cur;
        volatile double big = 1000.0, z = 0.0;
        double r;
        if (!cfs || !pe || !pl) return 78;
        SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS);
        if (c[0] == 'o') { cfs(&cur, 0, 0x04u); r = pe(big); }
        else if (c[0] == 'z') { cfs(&cur, 0, 0x08u); r = pl(z); }
        else r = pe(big);
        v_r = r;
        return (r == r && (r - r) != (r - r)) ? 0 : (r > 1e308 ? 0 : 77);
    }
    TC_RUN(overflow_unmasked); TC_RUN(zerodiv_unmasked); TC_RUN(control_masked);
    return tc_end();
}

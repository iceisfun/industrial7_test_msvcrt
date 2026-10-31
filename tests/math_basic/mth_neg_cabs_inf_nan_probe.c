/* TC
 * id:    mth_neg_cabs_inf_nan_probe
 * area:  math_basic
 * funcs:
 * procs: _cabs
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.4.3 hypot (inf wins over NaN); MS docs _cabs
 * note:  PROBE: C11 F.10.4.3 says hypot(+-inf, NaN) = +inf; whether _cabs follows that (vs returning NaN) is unknown.
 *        Asserts inf-wins; errno printed only.
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

typedef double (__cdecl *cabs_t)(struct _complex);
static cabs_t p_cabs;
static double C(double r, double i) { volatile double vr = r, vi = i; struct _complex c; c.x = vr; c.y = vi; return p_cabs(c); }
TC_CASE(inf_nan) { CHECK_BITS_D(C(pinf(), qnan()), pinf()); }
TC_CASE(nan_inf) { CHECK_BITS_D(C(qnan(), pinf()), pinf()); }
TC_CASE(errno_info) { double r; errno = 0; r = C(pinf(), qnan()); (void)r; info_i("errno_inf_nan", errno); }

int main(void) { tc_begin(); p_cabs = (cabs_t)tc_crt_proc("_cabs"); if (!p_cabs) { tc_write("TC FAIL " TC_TEST_ID "::_cabs not exported\n"); return 1; } TC_RUN(inf_nan); TC_RUN(nan_inf); TC_RUN(errno_info); return tc_end(); }

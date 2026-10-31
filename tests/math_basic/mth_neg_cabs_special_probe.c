/* TC
 * id:    mth_neg_cabs_special_probe
 * area:  math_basic
 * funcs:
 * procs: _cabs
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.4.3 hypot; MS docs _cabs
 * note:  PROBE: _cabs(NaN, 1) => NaN; _cabs(-inf,-inf) => +inf. (inf-with-NaN is in mth_neg_cabs_inf_nan_probe.)
 *        errno after the inf result is printed only.
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
TC_CASE(nan_one) { CHECK_IS_NAN(C(qnan(), 1.0)); }
TC_CASE(neginf_neginf) { CHECK_BITS_D(C(-pinf(), -pinf()), pinf()); }
TC_CASE(errno_info) { double r; errno = 0; r = C(pinf(), -pinf()); (void)r; info_i("errno_inf_inf", errno); }

int main(void) { tc_begin(); p_cabs = (cabs_t)tc_crt_proc("_cabs"); if (!p_cabs) { tc_write("TC FAIL " TC_TEST_ID "::_cabs not exported\n"); return 1; } TC_RUN(nan_one); TC_RUN(neginf_neginf); TC_RUN(errno_info); return tc_end(); }

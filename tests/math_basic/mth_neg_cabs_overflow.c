/* TC
 * id:    mth_neg_cabs_overflow
 * area:  math_basic
 * funcs:
 * procs: _cabs
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _cabs (overflow returns HUGE_VAL, errno ERANGE)
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

typedef double (__cdecl *cabs_t)(struct _complex);
static cabs_t p_cabs;
static volatile double v_zero = 0.0, v_one = 1.0;
static double pinf(void) { return v_one / v_zero; }
static void note_errno(const char *what, int e) { char b[24]; tc_i64(e, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write(" errno="); tc_write(b); tc_write("\n"); }

TC_CASE(overflow) { struct _complex z; double r; z.x = DBL_MAX; z.y = DBL_MAX; { volatile double t = z.x; z.x = t; } errno = 0; r = p_cabs(z); CHECK_BITS_D(r, pinf()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(finite_no_errno) { struct _complex z; z.x = 3; z.y = -4; { volatile double t = z.x; z.x = t; } errno = 0; CHECK_BITS_D(p_cabs(z), 5.0); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); p_cabs = (cabs_t)tc_crt_proc("_cabs"); if (!p_cabs) { tc_write("TC FAIL " TC_TEST_ID "::__import _cabs missing\n"); return 1; } TC_RUN(overflow); TC_RUN(finite_no_errno); return tc_end(); }

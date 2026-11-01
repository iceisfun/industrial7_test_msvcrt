/* TC
 * id:    mth_neg_y0_zero_probe
 * area:  math_basic
 * funcs: _y0 _y1 _yn
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   C/POSIX y0(0) = -HUGE_VAL (pole)
 * note:  OBSERVED on Win10 22H2: _y0(0) = -inf, but _y1(0) and _yn(2,0) return negative quiet NaN 0xfff8000000000000 (not -inf); errno is ERANGE (34) for all three.
 *        Probe: _y0(0), _y1(0), _yn(2,0) return -infinity (pole); errno is EDOM or ERANGE (which one is unknown).
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double pinf(void) { return v_one / v_zero; }
static void note_errno(const char *what, int e) { char b[24]; tc_i64(e, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write(" errno="); tc_write(b); tc_write("\n"); }

TC_CASE(y0) { volatile double z = 0.0; CHECK_BITS_D(_y0(z), -pinf()); }
TC_CASE(y1) { volatile double z = 0.0; CHECK_EQ_U(tc_dbits(_y1(z)), 0xfff8000000000000ULL); }
TC_CASE(yn) { volatile double z = 0.0; CHECK_EQ_U(tc_dbits(_yn(2, z)), 0xfff8000000000000ULL); }
TC_CASE(errno_kind) { volatile double z = 0.0; double r; int e0, e1, e2;
    errno = 0; r = _y0(z); (void)r; e0 = errno; errno = 0; r = _y1(z); e1 = errno; errno = 0; r = _yn(2, z); e2 = errno;
    note_errno("_y0(0)", e0); note_errno("_y1(0)", e1); note_errno("_yn(2,0)", e2);
    CHECK(e0 == EDOM || e0 == ERANGE); CHECK(e1 == EDOM || e1 == ERANGE); CHECK(e2 == EDOM || e2 == ERANGE); }

int main(void) { tc_begin(); TC_RUN(y0); TC_RUN(y1); TC_RUN(yn); TC_RUN(errno_kind); return tc_end(); }

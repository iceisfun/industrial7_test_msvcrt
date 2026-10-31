/* TC
 * id:    mth_neg_errno_untouched_on_success
 * area:  math_basic
 * funcs: sin cos sqrt log exp fmod floor atan2 _hypot
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   C11 7.5p3, 7.12.1 (math errno only on domain/range errors)
 * note:  Successful in-domain math calls must leave errno unchanged (not even set to 0). The standard does not strictly forbid setting errno nonzero on success, hence inferred.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double pinf(void) { return v_one / v_zero; }
static void note_errno(const char *what, int e) { char b[24]; tc_i64(e, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write(" errno="); tc_write(b); tc_write("\n"); }

static void calls(void)
{
    volatile double a = 1.0, b = 2.0, c = 4.0, d = 5.0, e = 3.0, r = 0;
    r += sin(a); r += cos(a); r += sqrt(c); r += log(b); r += exp(a);
    r += fmod(d, e); r += floor(a); r += atan2(a, b); r += _hypot(e, c);
}
TC_CASE(sentinel_preserved) { errno = 12345; calls(); CHECK_EQ_I(errno, 12345); }
TC_CASE(zero_preserved) { errno = 0; calls(); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(sentinel_preserved); TC_RUN(zero_preserved); return tc_end(); }

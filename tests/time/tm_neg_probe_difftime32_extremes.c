/* TC
 * id:    tm_neg_probe_difftime32_extremes
 * area:  time
 * kind:  negative
 * funcs:
 * procs: _difftime32
 * crt:   >=90
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _difftime32 (time1 - time0 as double)
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). _difftime32(INT_MIN, INT_MAX) returns 0.0, not -4294967295.0: ucrt validates both times and returns 0.0 (errno EINVAL, no handler call) for any negative argument; valid 32-bit pairs are computed as double (difftime32(0,INT_MAX)=-2147483647.0). Docs/C11 7.27.2.2 only say double difference; Windows deviates for negative times.
 *        32-bit subtraction would wrap to 1.0).
 */
#include "tc.h"
#include <time.h>
#include <limits.h>

TC_DECL_PROC(double, _difftime32, (__time32_t, __time32_t));

TC_CASE(diff32)
{
    volatile __time32_t lo = INT_MIN, hi = INT_MAX; double r;
    if (!tc_p__difftime32) TC_SKIP("no export");
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
    errno = 0; r = tc_p__difftime32(lo, hi); CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, EINVAL);   /* negative argument rejected -> 0.0 */
    r = tc_p__difftime32(0, hi); CHECK_BITS_D(r, -2147483647.0);
    r = tc_p__difftime32(hi, 0); CHECK_BITS_D(r, 2147483647.0);
    r = tc_p__difftime32(hi, hi); CHECK_BITS_D(r, 0.0);
#else
    r = tc_p__difftime32(lo, hi); CHECK_BITS_D(r, -4294967295.0);
#endif
}
int main(void) { tc_begin(); TC_LOAD_PROC(_difftime32); TC_RUN(diff32); return tc_end(); }

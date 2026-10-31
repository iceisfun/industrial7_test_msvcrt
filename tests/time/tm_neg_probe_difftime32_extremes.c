/* TC
 * id:    tm_neg_probe_difftime32_extremes
 * area:  time
 * kind:  negative
 * funcs:
 * procs: _difftime32
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _difftime32 (time1 - time0 as double)
 * note:  Probes that _difftime32(INT_MIN, INT_MAX) is computed without 32-bit overflow, i.e. -4294967295.0 (a naive
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
    r = tc_p__difftime32(lo, hi); CHECK_BITS_D(r, -4294967295.0);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_difftime32); TC_RUN(diff32); return tc_end(); }

/* TC
 * id:    m99_nearbyint_no_inexact
 * area:  math_c99
 * funcs: nearbyint rint
 * procs: _statusfp _clearfp
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.12.9.3 (nearbyint does not raise inexact); MS docs nearbyint
 * note:  PROBE: whether Windows nearbyint leaves _SW_INEXACT clear after nearbyint(2.5). C99 requires it; implementation unverified. rint(2.5) inexact state is recorded only (no assertion).
 */
#include "tc.h"
#include <math.h>
#include <float.h>

typedef unsigned int (__cdecl *fpfn_t)(void);
#define SW_INEXACT 0x1
static fpfn_t p_status, p_clear;

TC_CASE(nearbyint_no_inexact)
{
    volatile double x = 2.5; double r;
    if (!p_status || !p_clear) TC_SKIP("fp status not exported");
    p_clear();
    r = nearbyint(x);
    CHECK_BITS_D(r, 2.0);
    CHECK((p_status() & SW_INEXACT) == 0);
    p_clear();
}
TC_CASE(rint_value_only)
{
    volatile double x = 2.5; double r;
    if (!p_status || !p_clear) TC_SKIP("fp status not exported");
    p_clear();
    r = rint(x);
    CHECK_BITS_D(r, 2.0);   /* inexact flag state deliberately not asserted */
    p_clear();
}
int main(void)
{
    tc_begin();
    p_status = (fpfn_t)tc_crt_proc("_statusfp"); p_clear = (fpfn_t)tc_crt_proc("_clearfp");
    TC_RUN(nearbyint_no_inexact); TC_RUN(rint_value_only);
    return tc_end();
}

/* TC
 * id:    ucr_set_fma3_enable_probe
 * area:  ucrt_only
 * funcs:
 * procs: _set_FMA3_enable sqrt exp
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_FMA3_enable (x64 only)
 * note:  PROBE: _set_FMA3_enable(0) returns 0 and (1) afterwards returns 0/1 (recorded as >=0 only); results for exactly representable inputs unchanged.
 */
#include "tc.h"

typedef int (__cdecl *fma_t)(int);
typedef double (__cdecl *d1_t)(double);
TC_CASE(disable_and_math)
{
    fma_t f = (fma_t)tc_crt_proc("_set_FMA3_enable");
    volatile double four = 4.0, zero = 0.0; int r;
    d1_t sqrt_ = (d1_t)tc_crt_proc("sqrt"), exp_ = (d1_t)tc_crt_proc("exp");
    if (!f || !sqrt_ || !exp_) TC_SKIP("not exported");
    r = f(0);
    CHECK_EQ_I(r, 0);
    CHECK_BITS_D(sqrt_(four), 2.0);
    CHECK_BITS_D(exp_(zero), 1.0);
    r = f(1);
    CHECK(r == 0 || r == 1);
    CHECK_BITS_D(sqrt_(four), 2.0);
    CHECK_BITS_D(exp_(zero), 1.0);
}
int main(void) { tc_begin(); TC_RUN(disable_and_math); return tc_end(); }

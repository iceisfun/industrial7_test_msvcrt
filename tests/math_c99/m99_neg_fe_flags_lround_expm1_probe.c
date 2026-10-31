/* TC
 * id:    m99_neg_fe_flags_lround_expm1_probe
 * area:  math_c99
 * funcs: lround llround expm1 cbrt
 * procs: _statusfp _clearfp
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.6.7 / F.10.3; MS docs _statusfp, _clearfp
 * note:  PROBE: which IEEE exception flags (seen through _statusfp after _clearfp) are raised by failing
 *        C99 calls. C Annex F says lround(NaN) is invalid, expm1(710) overflows, erfc(30) underflows,
 *        nextafter(DBL_MAX,inf) overflows; whether the MS CRT sets the hardware/status flags is unverified.
 *        round() on exact/NaN input must not set INEXACT/INVALID. Every case is an independent guess.
 */
#include "tc.h"
#include <math.h>
#include <float.h>

#define SW_INEXACT 0x1u
#define SW_UNDERFLOW 0x2u
#define SW_OVERFLOW 0x4u
#define SW_INVALID 0x10u
typedef unsigned int (__cdecl *fpfn_t)(void);
static fpfn_t p_status, p_clear;
static volatile double v_a, v_b;
static volatile long v_l; static volatile long long v_ll; static volatile double v_r;

#define NEED() do { if (!p_status || !p_clear) TC_SKIP("no _statusfp"); } while (0)

TC_CASE(lround_nan_invalid) { NEED(); v_a = NAN; p_clear(); v_l = lround(v_a); CHECK((p_status() & SW_INVALID) != 0); }
TC_CASE(llround_nan_invalid) { NEED(); v_a = NAN; p_clear(); v_ll = llround(v_a); CHECK((p_status() & SW_INVALID) != 0); }
TC_CASE(expm1_overflow) { NEED(); v_a = 710.0; p_clear(); v_r = expm1(v_a); CHECK(v_r == HUGE_VAL); CHECK((p_status() & SW_OVERFLOW) != 0); }
TC_CASE(expm1_overflow_inexact) { NEED(); v_a = 710.0; p_clear(); v_r = expm1(v_a); CHECK((p_status() & SW_INEXACT) != 0); }
TC_CASE(cbrt_exact_clean) { NEED(); v_a = 27.0; p_clear(); v_r = cbrt(v_a); CHECK(v_r == 3.0); CHECK((p_status() & (SW_INVALID|SW_OVERFLOW|SW_UNDERFLOW)) == 0); }

int main(void)
{
    tc_begin();
    p_status = (fpfn_t)tc_crt_proc("_statusfp"); p_clear = (fpfn_t)tc_crt_proc("_clearfp");
    TC_RUN(lround_nan_invalid); TC_RUN(llround_nan_invalid); TC_RUN(expm1_overflow); TC_RUN(expm1_overflow_inexact); TC_RUN(cbrt_exact_clean);
    if (p_clear) p_clear();
    return tc_end();
}

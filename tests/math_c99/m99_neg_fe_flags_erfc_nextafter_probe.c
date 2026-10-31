/* TC
 * id:    m99_neg_fe_flags_erfc_nextafter_probe
 * area:  math_c99
 * funcs: erfc round
 * procs: _statusfp _clearfp nextafter
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.6.7 / F.10.3; MS docs _statusfp, _clearfp
 * note:  PROBE: which IEEE exception flags (seen through _statusfp after _clearfp) are raised by failing
 *        C99 calls. C Annex F says erfc(30) underflows,
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
typedef double (__cdecl *nafn_t)(double, double);
static nafn_t p_nextafter;
static volatile double v_a, v_b;
static volatile long v_l; static volatile long long v_ll; static volatile double v_r;

#define NEED() do { if (!p_status || !p_clear) TC_SKIP("no _statusfp"); } while (0)

TC_CASE(erfc_underflow) { NEED(); v_a = 30.0; p_clear(); v_r = erfc(v_a); CHECK(v_r == 0.0); CHECK((p_status() & SW_UNDERFLOW) != 0); }
TC_CASE(nextafter_max_overflow) { NEED(); v_a = DBL_MAX; v_b = HUGE_VAL; p_clear(); v_r = (p_nextafter ? p_nextafter(v_a, v_b) : 0.0); CHECK(v_r == HUGE_VAL); CHECK((p_status() & SW_OVERFLOW) != 0); }
TC_CASE(nextafter_zero_underflow) { NEED(); v_a = 0.0; v_b = 1.0; p_clear(); v_r = (p_nextafter ? p_nextafter(v_a, v_b) : 0.0); CHECK(v_r == 4.9406564584124654e-324); CHECK((p_status() & SW_UNDERFLOW) != 0); }
TC_CASE(round_exact_no_inexact) { NEED(); v_a = 2.5; p_clear(); v_r = round(v_a); CHECK(v_r == 3.0); CHECK((p_status() & SW_INEXACT) == 0); }
TC_CASE(round_nan_quiet) { NEED(); v_a = NAN; p_clear(); v_r = round(v_a); CHECK(v_r != v_r); CHECK((p_status() & SW_INVALID) == 0); }

int main(void)
{
    tc_begin();
    p_status = (fpfn_t)tc_crt_proc("_statusfp"); p_clear = (fpfn_t)tc_crt_proc("_clearfp");
    p_nextafter = (nafn_t)tc_crt_proc("nextafter");
    TC_RUN(erfc_underflow); TC_RUN(nextafter_max_overflow); TC_RUN(nextafter_zero_underflow); TC_RUN(round_exact_no_inexact); TC_RUN(round_nan_quiet);
    if (p_clear) p_clear();
    return tc_end();
}

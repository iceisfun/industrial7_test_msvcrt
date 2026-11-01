/* TC
 * id:    gmb_bessel_neg_j_special_probe
 * area:  math
 * funcs: _j0 _j1 _jn _y0
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   Bessel at non-finite arguments
 * note:  OBSERVED on Win10 22H2: _j0(+-inf), _j1(+inf), _jn(2,+inf) all return the negative quiet NaN 0xfff8000000000000 (not 0); _y0(+-inf) is NaN as well.
 *        PROBE: J(+-inf)==0 (may be NaN/EDOM on old CRTs); NaN in gives NaN out; _y0(-inf) non-finite (docs say HUGE_VAL for negative x). One case per function.
 */
#include "tc.h"
#include <math.h>

static volatile double v_zero = 0.0;
static double pinf(void) { return 1.0 / v_zero; }

TC_CASE(j0_inf) { double i = pinf(); CHECK_IS_NAN(_j0(i)); CHECK_IS_NAN(_j0(-i)); }
TC_CASE(j1_inf) { double i = pinf(); CHECK_IS_NAN(_j1(i)); }
TC_CASE(jn_inf) { double i = pinf(); CHECK_IS_NAN(_jn(2, i)); }
TC_CASE(j0_nan) { double n = pinf() - pinf(); CHECK_IS_NAN(_j0(n)); }
TC_CASE(y0_nan) { double n = pinf() - pinf(); CHECK_IS_NAN(_y0(n)); }
TC_CASE(y0_neginf) { double i = pinf(); CHECK(((tc_dbits(_y0(-i)) >> 52) & 0x7FF) == 0x7FF); }
int main(void) { tc_begin(); TC_RUN(j0_inf); TC_RUN(j1_inf); TC_RUN(jn_inf); TC_RUN(j0_nan); TC_RUN(y0_nan); TC_RUN(y0_neginf); return tc_end(); }

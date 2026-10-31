/* TC
 * id:    gmb_chgsign_nan
 * area:  math
 * funcs: _chgsign
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _chgsign
 * note:  PROBE: sign flip of quiet NaNs (docs only say 'reverses sign'). Signalling-NaN payload preservation is NOT asserted; only the sign flip and NaN-ness of quiet NaNs.
 */
#include "tc.h"
#include <float.h>
#include <math.h>

static double from_bits(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }

TC_CASE(pos_qnan)
{
    volatile double x = from_bits(0x7FF8000000000000ULL); double r = _chgsign(x);
    CHECK_IS_NAN(r);
    CHECK_EQ_U(tc_dbits(r) ^ tc_dbits(x), 1ULL << 63);
}
TC_CASE(neg_qnan)
{
    volatile double x = from_bits(0xFFF8000000000000ULL); double r = _chgsign(x);
    CHECK_IS_NAN(r);
    CHECK_EQ_U(tc_dbits(r) >> 63, 0);
}
int main(void) { tc_begin(); TC_RUN(pos_qnan); TC_RUN(neg_qnan); return tc_end(); }

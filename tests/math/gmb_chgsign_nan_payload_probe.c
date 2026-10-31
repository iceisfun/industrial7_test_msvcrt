/* TC
 * id:    gmb_chgsign_nan_payload_probe
 * area:  math
 * funcs: _chgsign
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _chgsign
 * note:  PROBE: quiet-NaN payload bits (other than sign) are preserved by _chgsign.
 */
#include "tc.h"
#include <float.h>
#include <math.h>

static double from_bits(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }

TC_CASE(payload_probe)
{
    volatile double x = from_bits(0x7FF8000000001234ULL); double r = _chgsign(x);
    CHECK_EQ_U(tc_dbits(r) & 0x7FFFFFFFFFFFFFFFULL, 0x7FF8000000001234ULL);
}
int main(void) { tc_begin(); TC_RUN(payload_probe); return tc_end(); }

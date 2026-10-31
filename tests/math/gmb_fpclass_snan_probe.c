/* TC
 * id:    gmb_fpclass_snan_probe
 * area:  math
 * funcs: _fpclass
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _fpclass (_FPCLASS_SNAN 0x0001)
 * note:  PROBE: a signalling NaN (0x7FF0000000000001) is classified _FPCLASS_SNAN. Depends on the sNaN surviving the argument pass in xmm0 (x64) un-quieted.
 */
#include "tc.h"
#include <float.h>
static double from_bits(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_CASE(snan) { volatile double s = from_bits(0x7FF0000000000001ULL), ns = from_bits(0xFFF0000000000001ULL); CHECK_EQ_I(_fpclass(s), 0x01); CHECK_EQ_I(_fpclass(ns), 0x01); }
int main(void) { tc_begin(); TC_RUN(snan); return tc_end(); }

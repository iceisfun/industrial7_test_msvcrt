/* TC
 * id:    mf_fpclassf_snan_probe
 * area:  math_float
 * funcs: _fpclassf
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _fpclass (_FPCLASS_SNAN 0x0001)
 * note:  PROBE: _fpclassf(signalling NaN 0x7F800001) == _FPCLASS_SNAN (0x01). May be 0x02 if implemented by float->double
 *        conversion (which quietens sNaN).
 */
#include "tc.h"
#include <float.h>
static float mk(unsigned int b) { union { unsigned int u; float f; } x; x.u = b; return x.f; }
static volatile unsigned int b_snan = 0x7F800001u;
TC_CASE(snan) { CHECK_EQ_I(_fpclassf(mk(b_snan)), 0x01); }
int main(void) { tc_begin(); TC_RUN(snan); return tc_end(); }

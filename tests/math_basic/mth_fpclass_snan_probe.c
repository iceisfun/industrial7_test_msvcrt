/* TC
 * id:    mth_fpclass_snan_probe
 * area:  math_basic
 * funcs: _fpclass
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _fpclass (_FPCLASS_SNAN)
 * note:  probing _fpclass SNAN detection for sNaN bit patterns passed by value in xmm0 (no x87 conversion on x64).
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static double qnan(void) { return mk(0x7FF8000000000000ULL); }
static double pinf(void) { return mk(0x7FF0000000000000ULL); }

TC_CASE(snan_pos) { volatile double x = mk(0x7FF0000000000001ULL); CHECK_EQ_I(_fpclass(x), 0x1); }
TC_CASE(snan_neg) { volatile double x = mk(0xFFF0000000000001ULL); CHECK_EQ_I(_fpclass(x), 0x1); }
TC_CASE(qnan_) { volatile double x = mk(0x7FF8000000000000ULL); CHECK_EQ_I(_fpclass(x), 0x2); }
int main(void) { tc_begin(); TC_RUN(snan_pos); TC_RUN(snan_neg); TC_RUN(qnan_); return tc_end(); }

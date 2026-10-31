/* TC
 * id:    gmb_fpclass_basic
 * area:  math
 * funcs: _fpclass
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _fpclass; float.h
 */
#include "tc.h"

static double from_bits(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }

TC_CASE(zeros) { volatile double p = 0.0, n = -0.0; CHECK_EQ_I(_fpclass(p), 0x40); CHECK_EQ_I(_fpclass(n), 0x20); }
TC_CASE(normals) { volatile double p = 1.0, n = -1.0; CHECK_EQ_I(_fpclass(p), 0x100); CHECK_EQ_I(_fpclass(n), 0x08); }
TC_CASE(denormals) { volatile double p = from_bits(1ULL), n = from_bits(0x8000000000000001ULL); CHECK_EQ_I(_fpclass(p), 0x80); CHECK_EQ_I(_fpclass(n), 0x10); }
TC_CASE(infinities) { volatile double p = from_bits(0x7FF0000000000000ULL), n = from_bits(0xFFF0000000000000ULL); CHECK_EQ_I(_fpclass(p), 0x200); CHECK_EQ_I(_fpclass(n), 0x04); }
TC_CASE(nans) { volatile double q = from_bits(0x7FF8000000000000ULL), nq = from_bits(0xFFF8000000000000ULL); CHECK_EQ_I(_fpclass(q), 0x02); CHECK_EQ_I(_fpclass(nq), 0x02); }
int main(void) { tc_begin(); TC_RUN(zeros); TC_RUN(normals); TC_RUN(denormals); TC_RUN(infinities); TC_RUN(nans); return tc_end(); }

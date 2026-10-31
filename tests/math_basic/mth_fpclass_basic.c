/* TC
 * id:    mth_fpclass_basic
 * area:  math_basic
 * funcs: _fpclass
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _fpclass; float.h _FPCLASS_*
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static double qnan(void) { return mk(0x7FF8000000000000ULL); }
static double pinf(void) { return mk(0x7FF0000000000000ULL); }

TC_CASE(zeros) { volatile double p = 0.0, n = -0.0; CHECK_EQ_I(_fpclass(p), 0x40); CHECK_EQ_I(_fpclass(n), 0x20); }
TC_CASE(normals) { volatile double p = 1.0, n = -1.0; CHECK_EQ_I(_fpclass(p), 0x100); CHECK_EQ_I(_fpclass(n), 0x08); }
TC_CASE(denormals) { volatile double p = mk(1ULL), n = mk(0x8000000000000001ULL); CHECK_EQ_I(_fpclass(p), 0x80); CHECK_EQ_I(_fpclass(n), 0x10); }
TC_CASE(infs) { volatile double p = pinf(), n = -pinf(); CHECK_EQ_I(_fpclass(p), 0x200); CHECK_EQ_I(_fpclass(n), 0x04); }
TC_CASE(qnan_) { volatile double q = qnan(); CHECK_EQ_I(_fpclass(q), 0x02); }
TC_CASE(macro_values) { CHECK_EQ_I(_FPCLASS_SNAN, 1); CHECK_EQ_I(_FPCLASS_QNAN, 2); CHECK_EQ_I(_FPCLASS_NINF, 4); CHECK_EQ_I(_FPCLASS_NN, 8); CHECK_EQ_I(_FPCLASS_ND, 0x10); CHECK_EQ_I(_FPCLASS_NZ, 0x20); CHECK_EQ_I(_FPCLASS_PZ, 0x40); CHECK_EQ_I(_FPCLASS_PD, 0x80); CHECK_EQ_I(_FPCLASS_PN, 0x100); CHECK_EQ_I(_FPCLASS_PINF, 0x200); }
int main(void) { tc_begin(); TC_RUN(zeros); TC_RUN(normals); TC_RUN(denormals); TC_RUN(infs); TC_RUN(qnan_); TC_RUN(macro_values); return tc_end(); }

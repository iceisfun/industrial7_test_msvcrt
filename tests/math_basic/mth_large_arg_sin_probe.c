/* TC
 * id:    mth_large_arg_sin_probe
 * area:  math_basic
 * funcs:
 * procs: sin cos
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs sin/cos
 * note:  probing accurate argument reduction: sin(1e22)=-0.8522008497671888, cos(1e22)=0.5232147853951389 within 1e-12; x64 CRTs reduce accurately, x87-based x86 would not. Observed on an FMA3/AVX2 CPU (ucrtbase dispatches FMA3 paths); non-FMA path unverified.
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <float.h>
#include <errno.h>
/* F: call the DLL export, never the libmingwex static copy (tc_crt_proc = GetProcAddress) */
TC_DECL_PROC(double, sin, (double));
#undef sin
#define sin tc_p_sin
TC_DECL_PROC(double, cos, (double));
#undef cos
#define cos tc_p_cos

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static double qnan(void) { return mk(0x7FF8000000000000ULL); }
static double pinf(void) { return mk(0x7FF0000000000000ULL); }

TC_CASE(sin_1e22) { volatile double x = 1e22; CHECK_NEAR(sin(x), -0.8522008497671888, 1e-12); }
TC_CASE(cos_1e22) { volatile double x = 1e22; CHECK_NEAR(cos(x), 0.5232147853951389, 1e-12); }
TC_CASE(sin_2p53) { volatile double x = 9007199254740992.0; CHECK_NEAR(sin(x), -0.848925964814655, 1e-12); }
int main(void) { tc_begin(); TC_LOAD_PROC(sin); TC_LOAD_PROC(cos); if (!tc_p_sin || !tc_p_cos) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } errno = 0; TC_RUN(sin_1e22); TC_RUN(cos_1e22); TC_RUN(sin_2p53); return tc_end(); }

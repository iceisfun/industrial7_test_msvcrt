/* TC
 * id:    mth_signed_zero_funcs
 * area:  math_basic
 * funcs: sqrt floor ceil
 * procs: fabs
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.4.5 (sqrt(-0)=-0), 7.12.9.1/7.12.9.2 (floor/ceil -0), F.3 (fabs)
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <float.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static double qnan(void) { return mk(0x7FF8000000000000ULL); }
static double pinf(void) { return mk(0x7FF0000000000000ULL); }

static double (__cdecl *p_fabs)(double);
#define NZ (-0.0)
TC_CASE(sqrt_) { volatile double x = NZ; CHECK_BITS_D(sqrt(x), NZ); }
TC_CASE(floor_ceil) { volatile double x = NZ; CHECK_BITS_D(floor(x), NZ); CHECK_BITS_D(ceil(x), NZ); }
TC_CASE(fabs_) { volatile double x = NZ; CHECK(p_fabs != NULL); if (p_fabs) CHECK_BITS_D(p_fabs(x), 0.0); }
int main(void) { tc_begin(); errno = 0; p_fabs = (double (__cdecl *)(double))tc_crt_proc("fabs"); TC_RUN(sqrt_); TC_RUN(floor_ceil); TC_RUN(fabs_); return tc_end(); }

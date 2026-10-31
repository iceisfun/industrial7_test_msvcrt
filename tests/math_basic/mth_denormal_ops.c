/* TC
 * id:    mth_denormal_ops
 * area:  math_basic
 * funcs: ldexp floor ceil fmod modf
 * procs: fabs frexp
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.6.4, 7.12.6.6, F.10.6
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
static double (__cdecl *p_frexp)(double, int *);
TC_CASE(fabs_) { volatile double d = -mk(1ULL); CHECK(p_fabs != NULL); if (p_fabs) CHECK_BITS_D(p_fabs(d), mk(1ULL)); }
TC_CASE(frexp_) { volatile double d = mk(1ULL); int e = 0; double m; CHECK(p_frexp != NULL); if (!p_frexp) return; m = p_frexp(d, &e); CHECK_BITS_D(m, 0.5); CHECK_EQ_I(e, -1073); }
TC_CASE(ldexp_) { volatile double d = mk(1ULL); volatile int e = 1074; CHECK_BITS_D(ldexp(d, e), 1.0); }
TC_CASE(floor_ceil) { volatile double d = mk(1ULL); CHECK_BITS_D(floor(d), 0.0); CHECK_BITS_D(ceil(d), 1.0); }
TC_CASE(fmod_) { volatile double d = mk(1ULL); CHECK_BITS_D(fmod(d, d), 0.0); }
TC_CASE(modf_) { volatile double d = mk(1ULL); double ip = 5.0; double fr = modf(d, &ip); CHECK_BITS_D(fr, mk(1ULL)); CHECK_BITS_D(ip, 0.0); }
int main(void) { tc_begin(); errno = 0; p_fabs = (double (__cdecl *)(double))tc_crt_proc("fabs"); p_frexp = (double (__cdecl *)(double, int *))tc_crt_proc("frexp"); TC_RUN(fabs_); TC_RUN(frexp_); TC_RUN(ldexp_); TC_RUN(floor_ceil); TC_RUN(fmod_); TC_RUN(modf_); return tc_end(); }

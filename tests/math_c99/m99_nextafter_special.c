/* TC
 * id:    m99_nextafter_special
 * area:  math_c99
 * funcs: nextafterf
 * procs: nextafter
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.8.3, 7.12.11.3; MS docs nextafter
 * note:  
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

typedef double (__cdecl *nad_t)(double, double);
static nad_t p_nextafter;
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
#define DINF (v_one / v_zero)
#define FINF (f_one / f_zero)

TC_CASE(dmax_to_inf) { volatile double m = DBL_MAX, i = DINF; CHECK_BITS_D(p_nextafter(m, i), i); }
TC_CASE(inf_to_zero) { volatile double i = DINF, z = 0.0; CHECK_BITS_D(p_nextafter(i, z), DBL_MAX); CHECK_BITS_D(p_nextafter(-i, z), -DBL_MAX); }
TC_CASE(dmin_down) { volatile double m = DBL_MIN, z = 0.0; double r = p_nextafter(m, z); CHECK_EQ_U(tc_dbits(r), 0x000fffffffffffffULL); }
TC_CASE(nan_in) { volatile double n = DINF - DINF, o = 1.0; CHECK_IS_NAN(p_nextafter(n, o)); CHECK_IS_NAN(p_nextafter(o, n)); }
TC_CASE(float_max) { volatile float m = FLT_MAX, i = FINF; CHECK_BITS_F(nextafterf(m, i), i); }
TC_CASE(equal_args) { volatile double a = 1.5, b = 1.5; CHECK_BITS_D(p_nextafter(a, b), 1.5); }

int main(void) { tc_begin(); p_nextafter = (nad_t)tc_crt_proc("nextafter"); TC_RUN(dmax_to_inf); TC_RUN(inf_to_zero); TC_RUN(dmin_down); TC_RUN(nan_in); TC_RUN(float_max); TC_RUN(equal_args); return tc_end(); }

/* TC
 * id:    m99_nextafter_neg_overflow
 * area:  math_c99
 * funcs: nextafterf
 * procs: nextafter
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.11.3, F.10.8.3 (overflow: +-HUGE_VAL)
 * note:  errno is only constrained to {0,ERANGE}
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

#define ERR_OK() (errno == 0 || errno == ERANGE)

TC_CASE(pos) { volatile double m = DBL_MAX, i = DINF; double r; errno = 0; r = p_nextafter(m, i); CHECK_BITS_D(r, i); CHECK(ERR_OK()); }
TC_CASE(neg) { volatile double m = -DBL_MAX, i = -DINF; double r; errno = 0; r = p_nextafter(m, i); CHECK_BITS_D(r, i); CHECK(ERR_OK()); }
TC_CASE(float_pos) { volatile float m = FLT_MAX, i = FINF; float r; errno = 0; r = nextafterf(m, i); CHECK_BITS_F(r, i); CHECK(ERR_OK()); }
TC_CASE(float_neg) { volatile float m = -FLT_MAX, i = -FINF; float r; errno = 0; r = nextafterf(m, i); CHECK_BITS_F(r, i); CHECK(ERR_OK()); }

int main(void) { tc_begin(); p_nextafter = (nad_t)tc_crt_proc("nextafter"); TC_RUN(pos); TC_RUN(neg); TC_RUN(float_pos); TC_RUN(float_neg); return tc_end(); }

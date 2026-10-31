/* TC
 * id:    err_neg_math_fmod_zero_probe
 * area:  errno_param
 * kind:  negative
 * funcs: 
 * procs: fmod
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fmod (NaN when y is 0)
 * note:  PROBE: fmod(1.0, 0.0) returns NaN (documented) and sets errno to EDOM (not clearly documented; probed).
 */
#include "tc.h"
#include <errno.h>

typedef double (__cdecl *d2_t)(double, double);
static volatile double v_a, v_b;

TC_CASE(fmod_zero_nan) { d2_t f = (d2_t)tc_crt_proc("fmod"); double r; if (!f) TC_SKIP("fmod missing"); v_a = 1.0; v_b = 0.0; r = f(v_a, v_b); CHECK_IS_NAN(r); }
TC_CASE(fmod_zero_edom) { d2_t f = (d2_t)tc_crt_proc("fmod"); if (!f) TC_SKIP("fmod missing"); v_a = 1.0; v_b = 0.0; errno = 0; f(v_a, v_b); CHECK_EQ_I(errno, EDOM); }

int main(void) { tc_begin(); TC_RUN(fmod_zero_nan); TC_RUN(fmod_zero_edom); return tc_end(); }

/* TC
 * id:    m99_nextafter_zero_sign_probe
 * area:  math_c99
 * funcs: nextafterf
 * procs: nextafter
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.12.11.3 (if x equals y, returns y)
 * note:  PROBE: nextafter(+0,-0) returns y = -0 and nextafter(-0,+0) returns +0 per ISO C; whether the Windows implementation returns y or x for equal-comparing zeros is not documented.
 */
#include "tc.h"
#include <math.h>

typedef double (__cdecl *nad_t)(double, double);
typedef float (__cdecl *naf_t)(float, float);
static nad_t p_nextafter;

TC_CASE(pos_to_neg) { volatile double z = 0.0, nz = -0.0; CHECK_BITS_D(p_nextafter(z, nz), -0.0); }
TC_CASE(neg_to_pos) { volatile double z = 0.0, nz = -0.0; CHECK_BITS_D(p_nextafter(nz, z), 0.0); }
TC_CASE(float_pos_to_neg) { volatile float z = 0.0f, nz = -0.0f; CHECK_BITS_F(nextafterf(z, nz), -0.0f); }

int main(void) { tc_begin(); p_nextafter = (nad_t)tc_crt_proc("nextafter"); CHECK(p_nextafter != NULL); if (!p_nextafter) return tc_end(); TC_RUN(pos_to_neg); TC_RUN(neg_to_pos); TC_RUN(float_pos_to_neg); return tc_end(); }

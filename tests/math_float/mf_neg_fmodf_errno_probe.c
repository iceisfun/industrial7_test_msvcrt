/* TC
 * id:    mf_neg_fmodf_errno_probe
 * area:  math_float
 * funcs:
 * procs: fmodf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.1, F.10.7.1; MS docs fmod
 * note:  Calls go to the real ucrtbase export via GetProcAddress (a bare call in a mingw exe lands in libmingwex static code, not the DLL).
 *        PROBE: errno after fmodf(1,0) and fmodf(inf,2). Expected EDOM (domain error); Windows may leave errno untouched.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
TC_DECL_PROC(float, fmodf, (float, float));
#undef fmodf
#define fmodf tc_p_fmodf

static volatile float v_zero = 0.0f, v_one = 1.0f;

TC_CASE(div_zero) { volatile float x = 1.0f, y = 0.0f; volatile float r; errno = 0; r = fmodf(x, y); (void)r; CHECK_EQ_I(errno, EDOM); }
TC_CASE(inf_x) { volatile float x = v_one / v_zero, y = 2.0f; volatile float r; errno = 0; r = fmodf(x, y); (void)r; CHECK_EQ_I(errno, EDOM); }

int main(void) { tc_begin(); TC_LOAD_PROC(fmodf); if (!tc_p_fmodf) { tc_write("TC FAIL " TC_TEST_ID "::proc fmodf not exported\n"); return 1; } TC_RUN(div_zero); TC_RUN(inf_x); return tc_end(); }

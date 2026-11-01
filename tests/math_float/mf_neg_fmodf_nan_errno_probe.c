/* TC
 * id:    mf_neg_fmodf_nan_errno_probe
 * area:  math_float
 * funcs:
 * procs: fmodf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.7.1; MS docs fmod
 * note:  probes errno of fmodf: NaN operands leave errno unchanged, while x mod +-0 and +-inf mod y are domain Observed on an FMA3/AVX2 CPU (ucrtbase dispatches FMA3 paths); non-FMA path unverified.
 *        errors (EDOM). Windows errno policy is not documented per case.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>
/* F: call the DLL export, never the libmingwex static copy (tc_crt_proc = GetProcAddress) */
TC_DECL_PROC(float, fmodf, (float, float));
#undef fmodf
#define fmodf tc_p_fmodf

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;
static float fb(unsigned int u) { union { float f; unsigned int u; } x; x.u = u; return x.f; }
#define SENT 1234

TC_CASE(nan_x_errno_unchanged) { volatile float x = v_nan, y = 2.0f, r; errno = SENT; r = fmodf(x, y); CHECK_IS_NAN(r); CHECK_EQ_I(errno, SENT); }
TC_CASE(nan_y_errno_unchanged) { volatile float x = 2.0f, y = v_nan, r; errno = SENT; r = fmodf(x, y); CHECK_IS_NAN(r); CHECK_EQ_I(errno, SENT); }
TC_CASE(x_mod_negzero_edom) { volatile float x = 2.0f, y = -0.0f, r; errno = 0; r = fmodf(x, y); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }
TC_CASE(ninf_x_edom) { volatile float x = -v_inf, y = 2.0f, r; errno = 0; r = fmodf(x, y); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(fmodf); if (!tc_p_fmodf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } v_inf = v_one / v_zero; v_nan = fb(0x7fc00000);
    TC_RUN(nan_x_errno_unchanged); TC_RUN(nan_y_errno_unchanged); TC_RUN(x_mod_negzero_edom); TC_RUN(ninf_x_edom);
    return tc_end();
}

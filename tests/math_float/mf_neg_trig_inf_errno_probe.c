/* TC
 * id:    mf_neg_trig_inf_errno_probe
 * area:  math_float
 * funcs:
 * procs: sinf cosf tanf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs sin/cos/tan: errno EDOM for invalid input (float variants assumed)
 * note:  PROBE: whether ucrtbase sinf/cosf/tanf set errno=EDOM for +-inf input. Documented for the double versions; float assumed alike.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(float, sinf, (float));
#undef sinf
#define sinf tc_p_sinf
TC_DECL_PROC(float, cosf, (float));
#undef cosf
#define cosf tc_p_cosf
TC_DECL_PROC(float, tanf, (float));
#undef tanf
#define tanf tc_p_tanf

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;
static void init_specials(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(sinf_edom) { errno = 0; (void)sinf(v_inf); CHECK_EQ_I(errno, EDOM); }
TC_CASE(cosf_edom) { errno = 0; (void)cosf(v_inf); CHECK_EQ_I(errno, EDOM); }
TC_CASE(tanf_edom) { errno = 0; (void)tanf(v_inf); CHECK_EQ_I(errno, EDOM); }
int main(void) { tc_begin(); TC_LOAD_PROC(sinf); TC_LOAD_PROC(cosf); TC_LOAD_PROC(tanf); if (!(tc_p_sinf && tc_p_cosf && tc_p_tanf)) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } init_specials(); TC_RUN(sinf_edom); TC_RUN(cosf_edom); TC_RUN(tanf_edom); return tc_end(); }

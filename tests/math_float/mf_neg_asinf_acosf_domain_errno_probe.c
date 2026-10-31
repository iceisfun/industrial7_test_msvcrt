/* TC
 * id:    mf_neg_asinf_acosf_domain_errno_probe
 * area:  math_float
 * funcs:
 * procs: asinf acosf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs asin/acos: errno EDOM for |x|>1 (float variants assumed)
 * note:  PROBE: whether ucrtbase asinf/acosf set errno=EDOM for out-of-domain input.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(float, asinf, (float));
#undef asinf
#define asinf tc_p_asinf
TC_DECL_PROC(float, acosf, (float));
#undef acosf
#define acosf tc_p_acosf

TC_CASE(asinf_edom) { volatile float x = 2.0f; errno = 0; (void)asinf(x); CHECK_EQ_I(errno, EDOM); }
TC_CASE(acosf_edom) { volatile float x = -2.0f; errno = 0; (void)acosf(x); CHECK_EQ_I(errno, EDOM); }
int main(void) { tc_begin(); TC_LOAD_PROC(asinf); TC_LOAD_PROC(acosf); if (!(tc_p_asinf && tc_p_acosf)) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(asinf_edom); TC_RUN(acosf_edom); return tc_end(); }

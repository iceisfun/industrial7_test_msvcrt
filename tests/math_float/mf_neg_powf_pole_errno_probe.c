/* TC
 * id:    mf_neg_powf_pole_errno_probe
 * area:  math_float
 * funcs:
 * procs: powf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.4.4, 7.12.1; MS docs pow
 * note:  PROBE: errno after powf(+-0, negative odd/even int). Expected ERANGE (pole error); some CRTs may give EDOM. Result of ingest decides.
 */
#include "tc.h"
#include <math.h>

TC_DECL_PROC(float, powf, (float, float));
#define powf tc_p_powf
#include <errno.h>

TC_CASE(pos_zero_m1) { volatile float z = 0.0f, m = -1.0f; volatile float r; errno = 0; r = powf(z, m); (void)r; CHECK_EQ_I(errno, ERANGE); }
TC_CASE(neg_zero_m3) { volatile float z = -0.0f, m = -3.0f; volatile float r; errno = 0; r = powf(z, m); (void)r; CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_LOAD_PROC(powf); if (!tc_p_powf) { tc_write("TC FAIL " TC_TEST_ID "::powf not exported\n"); return 1; } TC_RUN(pos_zero_m1); TC_RUN(neg_zero_m3); return tc_end(); }

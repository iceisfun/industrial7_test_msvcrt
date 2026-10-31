/* TC
 * id:    mf_neg_powf_errno
 * area:  math_float
 * funcs:
 * procs: powf
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs pow, powf (errno EDOM / ERANGE)
 * note:  pole case (powf(0,-1)) is in mf_neg_powf_pole_errno_probe
 */
#include "tc.h"
#include <math.h>

TC_DECL_PROC(float, powf, (float, float));
#define powf tc_p_powf
#include <errno.h>

TC_CASE(domain) { volatile float a = -2.0f, b = 0.5f; volatile float r; errno = 0; r = powf(a, b); (void)r; CHECK_EQ_I(errno, EDOM); }
TC_CASE(overflow) { volatile float a = 10.0f, b = 100.0f; volatile float r; errno = 0; r = powf(a, b); (void)r; CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_LOAD_PROC(powf); if (!tc_p_powf) { tc_write("TC FAIL " TC_TEST_ID "::powf not exported\n"); return 1; } TC_RUN(domain); TC_RUN(overflow); return tc_end(); }

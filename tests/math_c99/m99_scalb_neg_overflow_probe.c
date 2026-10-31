/* TC
 * id:    m99_scalb_neg_overflow_probe
 * area:  math_c99
 * funcs: _scalb
 * procs: 
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _scalb
 * note:  PROBE: _scalb(1.0, 2000) overflow returns +infinity (HUGE_VAL) on every CRT generation; errno is only
 *        constrained to {0, ERANGE}.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
static volatile double v_a, v_zero = 0.0, v_one = 1.0;
TC_CASE(overflow) { v_a = 1; errno = 0; CHECK(_scalb(v_a, 2000) == v_one / v_zero); CHECK(errno == 0 || errno == ERANGE); }
int main(void) { tc_begin(); TC_RUN(overflow); return tc_end(); }

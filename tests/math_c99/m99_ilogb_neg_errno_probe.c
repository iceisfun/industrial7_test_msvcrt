/* TC
 * id:    m99_ilogb_neg_errno_probe
 * area:  math_c99
 * funcs: ilogb logb
 * procs: 
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.6.5 (domain/range error); MS docs ilogb
 * note:  PROBE: errno values after ilogb(0)=EDOM, logb(0)=ERANGE, ilogb(inf)=EDOM. Not documented; asserted from memory of the UCRT.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
static volatile double v_zero = 0.0, v_one = 1.0;
TC_CASE(ilogb0) { errno = 0; (void)ilogb(v_zero); CHECK_EQ_I(errno, EDOM); }
TC_CASE(logb0) { errno = 0; (void)logb(v_zero); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(ilogb_inf) { volatile double i = v_one / v_zero; errno = 0; (void)ilogb(i); CHECK_EQ_I(errno, EDOM); }
int main(void) { tc_begin(); TC_RUN(ilogb0); TC_RUN(logb0); TC_RUN(ilogb_inf); return tc_end(); }

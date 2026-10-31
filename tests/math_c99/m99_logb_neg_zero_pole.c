/* TC
 * id:    m99_logb_neg_zero_pole
 * area:  math_c99
 * funcs: logb logbf ilogb
 * procs: 
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.3.11 (logb(+-0) = -inf, divide-by-zero), 7.12.6.5
 * note:  errno asserted only in {0,EDOM,ERANGE}
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#define ERR_OK() (errno == 0 || errno == EDOM || errno == ERANGE)
static volatile double v_zero = 0.0, v_one = 1.0;
TC_CASE(pos_zero) { double r, ninf = -(v_one / v_zero); errno = 0; r = logb(v_zero); CHECK(r == ninf); CHECK(ERR_OK()); }
TC_CASE(neg_zero) { volatile double z = -0.0; double r, ninf = -(v_one / v_zero); errno = 0; r = logb(z); CHECK(r == ninf); CHECK(ERR_OK()); }
TC_CASE(float_zero) { volatile float z = 0.0f; float r, ninf = -(float)(v_one / v_zero); errno = 0; r = logbf(z); CHECK(r == ninf); CHECK(ERR_OK()); }
TC_CASE(ilogb_sentinels) { volatile double n = (v_one / v_zero) - (v_one / v_zero); errno = 0; CHECK_EQ_I(ilogb(v_zero), (int)0x80000000); CHECK(ERR_OK());
    errno = 0; CHECK_EQ_I(ilogb(n), 0x7fffffff); CHECK(ERR_OK()); }
int main(void) { tc_begin(); TC_RUN(pos_zero); TC_RUN(neg_zero); TC_RUN(float_zero); TC_RUN(ilogb_sentinels); return tc_end(); }

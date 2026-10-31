/* TC
 * id:    m99_fdim_neg_overflow
 * area:  math_c99
 * funcs: fdim fdimf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.12.1, 7.12.1 (range error returns HUGE_VAL); MS docs fdim
 * note:  errno accepted as 0 or ERANGE.
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

TC_CASE(double_overflow)
{
    volatile double m = DBL_MAX, nm = -DBL_MAX; double r;
    errno = 0; r = fdim(m, nm);
    CHECK_BITS_D(r, HUGE_VAL); CHECK(errno == 0 || errno == ERANGE);
}
TC_CASE(float_overflow)
{
    volatile float m = FLT_MAX, nm = -FLT_MAX; float r;
    errno = 0; r = fdimf(m, nm);
    CHECK_BITS_F(r, HUGE_VALF); CHECK(errno == 0 || errno == ERANGE);
}
int main(void) { tc_begin(); TC_RUN(double_overflow); TC_RUN(float_overflow); return tc_end(); }

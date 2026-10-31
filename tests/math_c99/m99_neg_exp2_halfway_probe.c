/* TC
 * id:    m99_neg_exp2_halfway_probe
 * area:  math_c99
 * funcs: exp2 exp2f
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.3.2; round-to-nearest-even of exact halfway results
 * note:  PROBE: exp2(-1075) is exactly half the smallest subnormal and should round to +0 (ties to even); exp2(-1074)==4.94e-324 exactly; same for float at -150/-149. An implementation may be off by one ulp here.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

#define ERR_OK() (errno == 0 || errno == ERANGE || errno == EDOM)

TC_CASE(double_edge)
{
    volatile double a = -1074.0, b = -1075.0;
    errno = 0;
    CHECK_EQ_U(tc_dbits(exp2(a)), 0x0000000000000001ULL);
    CHECK_EQ_U(tc_dbits(exp2(b)), 0x0000000000000000ULL);
    CHECK(ERR_OK());
}
TC_CASE(float_edge)
{
    volatile float a = -149.0f, b = -150.0f;
    errno = 0;
    CHECK_EQ_U(tc_fbits(exp2f(a)), 0x00000001u);
    CHECK_EQ_U(tc_fbits(exp2f(b)), 0x00000000u);
    CHECK(ERR_OK());
}
int main(void) { tc_begin(); TC_RUN(double_edge); TC_RUN(float_edge); return tc_end(); }

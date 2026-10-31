/* TC
 * id:    fio_neg_setmaxstdio_range
 * area:  stdio_file
 * funcs: _getmaxstdio _setmaxstdio
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _setmaxstdio (returns -1 on failure; newmax above 8192 invalid)
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(too_big)
{
    volatile int n = 8193; int before = _getmaxstdio();
    tc_iph_install();
    CHECK_EQ_I(_setmaxstdio(n), -1);
    CHECK_EQ_I(_getmaxstdio(), before);
}
TC_CASE(zero)
{
    volatile int n = 0; int before = _getmaxstdio();
    tc_iph_install();
    CHECK_EQ_I(_setmaxstdio(n), -1);
    CHECK_EQ_I(_getmaxstdio(), before);
}
TC_CASE(negative)
{
    volatile int n = -1; int before = _getmaxstdio();
    tc_iph_install();
    CHECK_EQ_I(_setmaxstdio(n), -1);
    CHECK_EQ_I(_getmaxstdio(), before);
}
int main(void) { tc_begin(); TC_RUN(too_big); TC_RUN(zero); TC_RUN(negative); return tc_end(); }

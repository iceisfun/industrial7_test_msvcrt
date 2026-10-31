/* TC
 * id:    nfx_set_error_mode_roundtrip
 * area:  negative_fatal
 * funcs: _set_error_mode
 * crt:   >=80
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _set_error_mode: returns the old setting; _REPORT_ERRMODE(3) only reports; default _OUT_TO_DEFAULT(0)
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(roundtrip)
{
    volatile int rep = 3, one = 1, zero = 0;
    CHECK_EQ_I(_set_error_mode(rep), 0);
    CHECK_EQ_I(_set_error_mode(one), 0);
    CHECK_EQ_I(_set_error_mode(rep), 1);
    CHECK_EQ_I(_set_error_mode(zero), 1);
    CHECK_EQ_I(_set_error_mode(rep), 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(roundtrip);
    return tc_end();
}

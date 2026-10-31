/* TC
 * id:    nfx_neg_set_error_mode_invalid
 * area:  negative_fatal
 * funcs: _set_error_mode _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_error_mode: returns -1 if an error occurs (mode out of range)
 * note:  Only the return value and that the mode is unchanged are asserted; errno/handler are in the _iph_probe test.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(out_of_range)
{
    volatile int v;
    _set_invalid_parameter_handler(tc_iph);
    v = 4;    CHECK_EQ_I(_set_error_mode(v), -1);
    v = -1;   CHECK_EQ_I(_set_error_mode(v), -1);
    v = 1000; CHECK_EQ_I(_set_error_mode(v), -1);
    v = 3;    CHECK_EQ_I(_set_error_mode(v), 0);  /* unchanged by the failed calls */
}
int main(void)
{
    tc_begin();
    TC_RUN(out_of_range);
    return tc_end();
}

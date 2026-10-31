/* TC
 * id:    err_neg_set_error_mode_invalid
 * area:  errno_param
 * kind:  negative
 * funcs: _set_error_mode _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_error_mode (returns -1 on error)
 * note:  PROBE: out-of-range modes (4, -1) return -1 and leave the mode unchanged. Handler count and errno are not asserted.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(mode4)  { volatile int m = 4; CHECK_EQ_I(_set_error_mode(m), -1); }
TC_CASE(modem1) { volatile int m = -1; CHECK_EQ_I(_set_error_mode(m), -1); }
TC_CASE(unchanged)
{
    volatile int q = 3, bad = 4; int before = _set_error_mode(q);
    _set_error_mode(bad);
    CHECK_EQ_I(_set_error_mode(q), before);
}
int main(void)
{
    tc_begin();
    _set_invalid_parameter_handler(tc_iph);
    TC_RUN(mode4); TC_RUN(modem1); TC_RUN(unchanged);
    return tc_end();
}

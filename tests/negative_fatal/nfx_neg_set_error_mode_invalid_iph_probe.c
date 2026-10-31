/* TC
 * id:    nfx_neg_set_error_mode_invalid_iph_probe
 * area:  negative_fatal
 * funcs: _set_error_mode _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs "Parameter validation"
 * note:  Probes that _set_error_mode(4) sets errno=EINVAL and calls the invalid-parameter handler exactly once.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(errno_and_handler)
{
    volatile int v = 4;
    _set_invalid_parameter_handler(tc_iph);
    errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(_set_error_mode(v), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_RUN(errno_and_handler);
    return tc_end();
}

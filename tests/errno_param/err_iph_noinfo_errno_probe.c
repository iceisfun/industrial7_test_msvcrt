/* TC
 * id:    err_iph_noinfo_errno_probe
 * area:  errno_param
 * funcs: _invalid_parameter_noinfo _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _invalid_parameter_noinfo
 * note:  PROBE: errno is set by callers, not by _invalid_parameter_noinfo itself; errno stays 0 across the call.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(errno_untouched)
{
    errno = 0; tc_iph_count = 0;
    _invalid_parameter_noinfo();
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, 0);
}
int main(void)
{
    tc_begin();
    _set_invalid_parameter_handler(tc_iph);
    TC_RUN(errno_untouched);
    return tc_end();
}

/* TC
 * id:    pfu_neg_sprintf_s_null_size0_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs sprintf_s (does not discuss sizeOfBuffer == 0)
 * note:  PROBING: sprintf_s(NULL, 0, "") is rejected like any NULL buffer (-1, handler once, EINVAL) rather than treated as a size query (_snprintf_s has a special NULL/0/0 case; sprintf_s is believed not to).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(null_size0)
{
    char *volatile p = NULL; int n;
    errno = 0; tc_iph_count = 0;
    n = sprintf_s(p, 0, "");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    TC_RUN(null_size0);
    return tc_end();
}

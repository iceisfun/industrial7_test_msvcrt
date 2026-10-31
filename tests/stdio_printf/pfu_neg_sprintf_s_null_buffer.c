/* TC
 * id:    pfu_neg_sprintf_s_null_buffer
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s: if buffer or format is a null pointer, invalid parameter handler; returns -1 and sets errno to EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(null_size10)
{
    char *volatile p = NULL; int n;
    errno = 0; tc_iph_count = 0;
    n = sprintf_s(p, 10, "x");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    TC_RUN(null_size10);
    return tc_end();
}

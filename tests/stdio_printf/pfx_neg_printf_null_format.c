/* TC
 * id:    pfx_neg_printf_null_format
 * area:  stdio_printf
 * kind:  negative
 * funcs: printf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf, _printf_l ("If format is a null pointer, the invalid parameter handler is invoked ... returns -1, errno EINVAL")
 * note:  nothing is written to the real stdout because the format is NULL.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(null_format)
{
    const char *volatile fmt = NULL; int r;
    if (!tc_iph_install()) TC_SKIP("no _set_invalid_parameter_handler");
    tc_iph_count = 0; errno = 0;
    r = printf(fmt);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(null_format); return tc_end(); }

/* TC
 * id:    pfm_3_neg_sprintf_null_buffer_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs sprintf, _sprintf_l, swprintf, _swprintf_l (null buffer/format invoke the invalid parameter handler; return -1, errno EINVAL)
 * note:  PROBING: sprintf(NULL, "abc") (non-_s) hits the invalid parameter handler, returns -1 with errno EINVAL as the docs imply; it is not treated as a counting call.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static char *volatile nullbuf = NULL;

TC_CASE(null_buffer)
{
    int n;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    n = sprintf(nullbuf, "abc");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(null_buffer); return tc_end(); }

/* TC
 * id:    pfm_3_neg_sprintf_null_format
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf, _sprintf_l, swprintf, _swprintf_l (null buffer/format invoke the invalid parameter handler; return -1, errno EINVAL)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile nullfmt = NULL;

TC_CASE(null_format)
{
    char b[16]; int n; b[0] = 'x'; b[1] = 0;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    n = sprintf(b, nullfmt);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(null_format); return tc_end(); }

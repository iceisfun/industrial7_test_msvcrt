/* TC
 * id:    pfr_neg_sprintf_null_buffer
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf, _sprintf_l (NULL buffer: invalid parameter handler, EINVAL, -1); Parameter validation
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
#include <string.h>

static int have_iph;
TC_CASE(null_buf_x)
{
    char *volatile nb = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = sprintf(nb, "x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buf_empty)
{
    char *volatile nb = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = sprintf(nb, "");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
    TC_RUN(null_buf_x); TC_RUN(null_buf_empty);
    return tc_end();
}

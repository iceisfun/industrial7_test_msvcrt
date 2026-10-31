/* TC
 * id:    pfs_neg_sprintf_s_zero_size
 * area:  stdio_printf
 * funcs: sprintf_s

 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s: sizeOfBuffer 0 is an invalid parameter, errno EINVAL

 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile f_x = "x";
TC_CASE(size0)
{
    char b[8]; int n, i; for (i = 0; i < 8; i++) b[i] = 'Z';
    errno = 0; tc_iph_count = 0;
    n = sprintf_s(b, 0, f_x);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(not_written)
{
    char b[8]; int n, i; for (i = 0; i < 8; i++) b[i] = 'Z';
    n = sprintf_s(b, 0, f_x);
    CHECK_EQ_I(n, -1);
    for (i = 0; i < 8; i++) CHECK(b[i] == 'Z');
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(size0); TC_RUN(not_written); return tc_end(); }

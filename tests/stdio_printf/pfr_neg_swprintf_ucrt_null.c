/* TC
 * id:    pfr_neg_swprintf_ucrt_null
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  documented
 * ref:   MS docs swprintf, swprintf_l (parameter validation: EINVAL, invalid parameter handler)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(null_buffer)
{
    wchar_t *volatile nb = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = swprintf(nb, 5, L"x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    wchar_t b[8]; const wchar_t *volatile nf = NULL; int r;
    b[0] = L'Q';
    tc_iph_count = 0; errno = 0;
    r = swprintf(b, 5, nf);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(null_buffer); TC_RUN(null_format);
    return tc_end();
}

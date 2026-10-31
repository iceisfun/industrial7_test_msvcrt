/* TC
 * id:    pff_neg_sprintf_null_buffer_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs sprintf (parameter validation)
 * note:  PROBE: non-_s sprintf also validates a NULL buffer / NULL format since VS2005: returns -1 and calls the invalid-parameter handler once.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(null_buffer)
{
    char * volatile b = NULL; volatile double v = 1.5; int r;
    tc_iph_count = 0; errno = 0;
    r = sprintf(b, "%f", v);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(null_format)
{
    char b[16]; const char * volatile f = NULL; int r;
    b[0] = 'x';
    tc_iph_count = 0; errno = 0;
    r = sprintf(b, f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_buffer); TC_RUN(null_format); return tc_end();
}

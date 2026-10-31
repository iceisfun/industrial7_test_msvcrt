/* TC
 * id:    pff_neg_fprintf_null_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fprintf, _fprintf_l: stream NULL -> invalid parameter handler, returns a negative value (-1)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(null_stream)
{
    FILE * volatile s = NULL; volatile double v = 1.5; int r;
    tc_iph_count = 0; errno = 0;
    r = fprintf(s, "%f", v);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_stream); return tc_end();
}

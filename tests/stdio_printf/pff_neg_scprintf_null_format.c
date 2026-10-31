/* TC
 * id:    pff_neg_scprintf_null_format
 * area:  stdio_printf
 * kind:  negative
 * funcs: _scprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _scprintf: NULL format -> invalid parameter handler, returns -1
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(null_format)
{
    const char * volatile f = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = _scprintf(f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(valid_float_counts)
{
    volatile double v = 1.5; int r;
    tc_iph_count = 0;
    r = _scprintf("%f", v);
    CHECK_EQ_I(r, 8);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_format); TC_RUN(valid_float_counts); return tc_end();
}

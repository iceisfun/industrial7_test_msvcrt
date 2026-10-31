/* TC
 * id:    pff_neg_snprintf_s_count_gt_size_float
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s: if the result does not fit sizeOfBuffer (count not _TRUNCATE) the invalid parameter handler runs, buffer becomes "" and -1 is returned; count < size truncates and returns -1
 * note:  The errno value of the overflow path is probed in pff_neg_probe_erange_float_overflow.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(overflow_f)
{
    char b[8]; volatile double v = 3.14159; int r, i;
    for (i = 0; i < 8; i++) b[i] = 'x';
    tc_iph_count = 0;
    r = _snprintf_s(b, 8, 100, "%f", v);   /* "3.141590" = 8 chars, needs 9 bytes */
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(count_lt_size_truncates)
{
    char b[8]; volatile double v = 3.14159; int r, i;
    for (i = 0; i < 8; i++) b[i] = 'x';
    tc_iph_count = 0;
    r = _snprintf_s(b, 8, 7, "%f", v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_STR(b, "3.14159");
}
TC_CASE(overflow_e)
{
    char b[8]; volatile double v = 12345.678901; int r, i;
    for (i = 0; i < 8; i++) b[i] = 'x';
    tc_iph_count = 0;
    r = _snprintf_s(b, 8, 100, "%e", v);   /* "1.234568e+04" */
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(overflow_f); TC_RUN(count_lt_size_truncates); TC_RUN(overflow_e);
    return tc_end();
}

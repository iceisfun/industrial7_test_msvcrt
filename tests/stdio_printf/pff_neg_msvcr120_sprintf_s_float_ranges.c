/* TC
 * id:    pff_neg_msvcr120_sprintf_s_float_ranges
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s _snprintf_s _ecvt_s
 * crt:   msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf_s (buffer too small: buf[0]=0, ERANGE, handler), _snprintf_s with _TRUNCATE (truncate, return -1), NULL buffer EINVAL
 * note:  msvcr120 parity with the ucrtbase secure float printf failures. The _ecvt_s case asserts only that a too-small buffer
 *        is an error (non-zero), which the docs state.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

TC_CASE(sprintf_s_small)
{
    char b[8]; volatile double v = 12345.678; int r; b[0] = 'x'; tc_iph_count = 0; errno = 0;
    r = sprintf_s(b, sizeof b, "%f", v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(sprintf_s_null)
{
    char * volatile b = NULL; int r; tc_iph_count = 0; errno = 0;
    r = sprintf_s(b, 8, "%f", 1.0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(snprintf_s_truncate)
{
    char b[8]; volatile double v = 12345.678; int r; tc_iph_count = 0;
    r = _snprintf_s(b, sizeof b, _TRUNCATE, "%f", v);
    CHECK_EQ_I(r, -1); CHECK_STR(b, "12345.6"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(ecvt_s_small)
{
    char b[2]; int d = 0, s = 0; errno_t r;
    r = _ecvt_s(b, 2, 1.0, 5, &d, &s);
    CHECK(r != 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(sprintf_s_small); TC_RUN(sprintf_s_null); TC_RUN(snprintf_s_truncate); TC_RUN(ecvt_s_small);
    return tc_end();
}

/* TC
 * id:    pff_neg_msvcr100_null_format_float
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf fprintf _snprintf _scprintf
 * crt:   msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf/fprintf/_snprintf/_scprintf: NULL format, NULL buffer (with nonzero count) or NULL stream -> invalid parameter handler, -1, errno EINVAL (VS2005+)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(sprintf_null_format) { char b[16]; const char * volatile f = NULL; int r; b[0] = 'x'; tc_iph_count = 0; errno = 0;
    r = sprintf(b, f); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(snprintf_null_buffer) { char * volatile b = NULL; int r; tc_iph_count = 0; errno = 0;
    r = _snprintf(b, 5, "%f", 1.0); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(scprintf_null_format) { const char * volatile f = NULL; int r; tc_iph_count = 0; errno = 0;
    r = _scprintf(f); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(fprintf_null_stream) { FILE * volatile fp = NULL; int r; tc_iph_count = 0; errno = 0;
    r = fprintf(fp, "%f", 1.0); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(sprintf_null_format); TC_RUN(snprintf_null_buffer); TC_RUN(scprintf_null_format); TC_RUN(fprintf_null_stream);
    return tc_end();
}

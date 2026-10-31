/* TC
 * id:    pff_neg_snprintf_s_null_args_float
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s, _snprintf_s_l: buffer or format NULL -> invalid parameter handler, -1, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define EXPECT_FAIL(expr) do { int r_; tc_iph_count = 0; errno = 0; r_ = (expr); \
    CHECK_EQ_I(r_, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(null_buffer)
{
    char * volatile nb = NULL; volatile double v = 1.0;
    EXPECT_FAIL(_snprintf_s(nb, 10, 5, "%f", v));
}
TC_CASE(null_format)
{
    char b[10]; const char * volatile nf = NULL;
    EXPECT_FAIL(_snprintf_s(b, 10, 5, nf));
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_buffer); TC_RUN(null_format);
    return tc_end();
}

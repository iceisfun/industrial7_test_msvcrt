/* TC
 * id:    pfo_neg_vsnprintf_s_null_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsnprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _snprintf_s
 * note:  PROBES (a) NULL buffer, size 8, count 4: expected -1, EINVAL, handler once;
 *        (b) NULL buffer, size 0, count 0: expected NO handler invocation (return value not asserted).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int cv(char *b, size_t n, size_t cnt, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsnprintf_s(0, b, n, cnt, f, NULL, ap); va_end(ap);
    return r;
}
TC_CASE(null_buf_nonzero_size)
{
    char * volatile b = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = cv(b, 8, 4, "%d", 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buf_zero_size_zero_count)
{
    char * volatile b = NULL; volatile size_t z = 0;
    tc_iph_count = 0;
    (void)cv(b, z, z, "%d", 1);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_buf_nonzero_size); TC_RUN(null_buf_zero_size_zero_count);
    return tc_end();
}

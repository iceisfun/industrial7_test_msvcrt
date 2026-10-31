/* TC
 * id:    pfo_neg_vsprintf_null_buf_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs sprintf ("buffer" null) - documented for sprintf_s, unverified for sprintf
 * note:  PROBES __stdio_common_vsprintf(opts=0, NULL buffer, n=16): expected -1, errno EINVAL, handler invoked once.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int cv(unsigned __int64 opts, char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsprintf(opts, b, n, f, NULL, ap); va_end(ap);
    return r;
}
TC_CASE(null_buf)
{
    char * volatile b = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = cv(0, b, 16, "%d", 5);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_buf);
    return tc_end();
}

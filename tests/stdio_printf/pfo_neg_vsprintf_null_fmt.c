/* TC
 * id:    pfo_neg_vsprintf_null_fmt
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf, sprintf_l: "If format is a null pointer, the invalid parameter handler is invoked"; snprintf with count 0 returns the required length (C99 7.21.6.5)
 * note:  size_zero_query uses the internal option 0x2 (_CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR from corecrt_stdio_config.h).
 *        Calls __stdio_common_vsprintf directly. NULL destination with n > 0 is in pfo_neg_vsprintf_null_buf_probe.
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

TC_CASE(null_format)
{
    char b[8]; const char * volatile f = NULL; int r;
    b[0] = 'Q'; tc_iph_count = 0; errno = 0;
    r = cv(0, b, sizeof b, f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format_snprintf_opts)
{
    char b[8]; const char * volatile f = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = cv(0x2, b, sizeof b, f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size_zero_query)
{
    char b[8]; volatile size_t n = 0; int r, i;
    for (i = 0; i < 8; i++) b[i] = 'Q';
    tc_iph_count = 0; errno = 0;
    r = cv(0x2, b, n, "%s-%d", "abc", 12);
    CHECK_EQ_I(r, 6);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(b[0], 'Q');
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_format); TC_RUN(null_format_snprintf_opts); TC_RUN(size_zero_query);
    return tc_end();
}

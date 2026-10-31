/* TC
 * id:    pfo_neg_vsprintf_null_args
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf/_vsnprintf: NULL format -> invalid parameter handler, errno EINVAL, return -1; C11 7.21.6.5 snprintf(NULL,0,...)
 * note:  invalid-parameter paths, handler installed so process survives.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int vs(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static int vw(unsigned long long o, wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile int vi = 42;

#include <errno.h>
static char b[16];

TC_CASE(null_format)
{
    int n;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0; errno = 0;
    n = vs(0, b, sizeof b, NULL);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buffer_zero_count_ok)
{
    int n;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0;
    n = vs(2, NULL, 0, "abc");
    CHECK_EQ_I(n, 3); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(null_format); TC_RUN(null_buffer_zero_count_ok);
    return tc_end();
}

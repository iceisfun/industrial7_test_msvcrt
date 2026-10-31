/* TC
 * id:    pfo_neg_probe_vsprintf_null_buffer
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _snprintf/_vsnprintf (invalid parameters)
 * note:  PROBE: NULL buffer with count != 0 is expected to hit the invalid-parameter handler (EINVAL, -1), both for options 0 and 2.
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

TC_CASE(null_buffer_nonzero_count)
{
    int n;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0; errno = 0;
    n = vs(0, NULL, 8, "abc");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buffer_std_snprintf_nonzero_count)
{
    int n;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0; errno = 0;
    n = vs(2, NULL, 8, "abc");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_RUN(null_buffer_nonzero_count); TC_RUN(null_buffer_std_snprintf_nonzero_count);
    return tc_end();
}

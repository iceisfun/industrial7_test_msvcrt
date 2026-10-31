/* TC
 * id:    pfi_neg_vsprintf_s_overflow
 * area:  stdio_printf
 * kind:  negative
 * funcs: vsprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vsprintf_s, Parameter validation: ERANGE / EINVAL, invalid parameter handler
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int call(char *b, size_t sz, const char *f, ...)
{ int r; va_list ap; va_start(ap, f); r = vsprintf_s(b, sz, f, ap); va_end(ap); return r; }

TC_CASE(small_buffer)
{
    char b[8]; int n; b[0] = 'x'; errno = 0; tc_iph_count = 0;
    n = call(b, 3, "%d", 12345);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(null_buffer)
{
    char *volatile p = NULL; int n; errno = 0; tc_iph_count = 0;
    n = call(p, 8, "%d", 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    char b[8]; const char *volatile f = NULL; int n; errno = 0; tc_iph_count = 0;
    n = call(b, 8, f, 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(zero_size)
{
    char b[8]; volatile size_t z = 0; int n; errno = 0; tc_iph_count = 0;
    n = call(b, z, "%d", 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(small_buffer); TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(zero_size);
    return tc_end();
}

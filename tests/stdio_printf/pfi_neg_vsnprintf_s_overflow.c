/* TC
 * id:    pfi_neg_vsnprintf_s_overflow
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsnprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _vsnprintf_s, _vsnprintf_s_l
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int call(char *b, size_t sz, size_t cnt, const char *f, ...)
{ int r; va_list ap; va_start(ap, f); r = _vsnprintf_s(b, sz, cnt, f, ap); va_end(ap); return r; }

TC_CASE(null_buffer)
{
    char *volatile p = NULL; int n; errno = 0; tc_iph_count = 0;
    n = call(p, 8, 4, "%d", 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    char b[8]; const char *volatile f = NULL; int n; errno = 0; tc_iph_count = 0;
    n = call(b, 8, 4, f, 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(truncate)
{
    char b[8]; int n, i; for (i = 0; i < 8; i++) b[i] = 'Z';
    tc_iph_count = 0;
    n = call(b, 4, _TRUNCATE, "%d", 987654);
    CHECK_EQ_I(n, -1); CHECK_STR(b, "987"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_gt_size_overflow)
{
    char b[4]; int n; b[0] = 'x'; errno = 0; tc_iph_count = 0;
    n = call(b, 4, 20, "%d", 987654);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(truncate); TC_RUN(count_gt_size_overflow);
    return tc_end();
}

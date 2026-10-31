/* TC
 * id:    pfi_neg_vswprintf_s_null
 * area:  stdio_printf
 * kind:  negative
 * funcs: vswprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vswprintf_s, vswprintf_s_l
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

static int call(wchar_t *b, size_t sz, const wchar_t *f, ...)
{ int r; va_list ap; va_start(ap, f); r = vswprintf_s(b, sz, f, ap); va_end(ap); return r; }

TC_CASE(null_buffer)
{
    wchar_t *volatile p = NULL; int n; errno = 0; tc_iph_count = 0;
    n = call(p, 8, L"%d", 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    wchar_t b[8]; const wchar_t *volatile f = NULL; int n; errno = 0; tc_iph_count = 0;
    n = call(b, 8, f, 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(overflow)
{
    wchar_t b[8]; int n; b[0] = L'x'; errno = 0; tc_iph_count = 0;
    n = call(b, 3, L"%d", 12345);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(overflow);
    return tc_end();
}

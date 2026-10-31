/* TC
 * id:    ucr_neg_stdio_vswprintf_null_buffer
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vswprintf_s
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs swprintf_s, vswprintf_s (buffer/format NULL or size <= 0: invalid parameter handler, EINVAL, returns -1)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

typedef int (__cdecl *vws_t)(unsigned long long, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vws_t p_vwss;
static int callws(wchar_t *b, size_t n, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vwss(0, b, n, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(null_buffer)
{
    int r;
    if (!p_vwss) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    r = callws(NULL, 4, L"x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    wchar_t b[8]; int r;
    if (!p_vwss) TC_SKIP("not exported");
    b[0] = L'Q'; errno = 0; tc_iph_count = 0;
    r = callws(b, 8, NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size_zero)
{
    wchar_t b[8]; int r;
    if (!p_vwss) TC_SKIP("not exported");
    b[0] = L'Q'; errno = 0; tc_iph_count = 0;
    r = callws(b, 0, L"x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vwss = (vws_t)tc_crt_proc("__stdio_common_vswprintf_s");
    TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(size_zero);
    return tc_end();
}

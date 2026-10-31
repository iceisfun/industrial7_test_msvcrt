/* TC
 * id:    ucr_neg_stdio_vsnprintf_s_wide_truncate
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsnprintf_s __stdio_common_vsnwprintf_s
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf_s, _snwprintf_s (_TRUNCATE: truncates, returns -1, no handler; count < size truncates silently with -1)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

typedef int (__cdecl *vsn_t)(unsigned long long, char *, size_t, size_t, const char *, void *, va_list);
typedef int (__cdecl *vwsn_t)(unsigned long long, wchar_t *, size_t, size_t, const wchar_t *, void *, va_list);
static vsn_t p_vsn; static vwsn_t p_vwsn;
static int calln(char *b, size_t n, size_t c, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vsn(0, b, n, c, fmt, NULL, ap); va_end(ap); return r; }
static int callwn(wchar_t *b, size_t n, size_t c, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vwsn(0, b, n, c, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(narrow_truncate_flag)
{
    char b[8]; int r;
    if (!p_vsn) TC_SKIP("not exported");
    memset(b, 'S', sizeof b); errno = 0; tc_iph_count = 0;
    r = calln(b, 4, (size_t)-1, "abcdef");
    CHECK_EQ_I(r, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(narrow_count_less_than_size)
{
    char b[8]; int r;
    if (!p_vsn) TC_SKIP("not exported");
    memset(b, 'S', sizeof b); tc_iph_count = 0;
    r = calln(b, 4, 3, "abcdef");
    CHECK_EQ_I(r, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_truncate_flag)
{
    wchar_t b[8]; int r, i;
    if (!p_vwsn) TC_SKIP("not exported");
    for (i = 0; i < 8; i++) b[i] = L'S';
    tc_iph_count = 0;
    r = callwn(b, 4, (size_t)-1, L"abcdef");
    CHECK_EQ_I(r, -1); CHECK_WSTR(b, L"abc"); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(b[4], L'S');
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vsn = (vsn_t)tc_crt_proc("__stdio_common_vsnprintf_s");
    p_vwsn = (vwsn_t)tc_crt_proc("__stdio_common_vsnwprintf_s");
    TC_RUN(narrow_truncate_flag); TC_RUN(narrow_count_less_than_size); TC_RUN(wide_truncate_flag);
    return tc_end();
}

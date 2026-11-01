/* TC
 * id:    ucr_neg_stdio_vsprintf_trailing_pct_probe
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsprintf __stdio_common_vswprintf
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs printf format specification
 * note:  Win10 22H2 (observed): a format that ends in the middle of a conversion specification ("%", "ab%5", wide "%") is not an error in
 *        __stdio_common_vsprintf/vswprintf (options 0): the incomplete spec is dropped, the output is the text before it ("", "ab", ""), return 0/2/0, errno untouched, no handler call.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

typedef int (__cdecl *vs_t)(unsigned long long, char *, size_t, const char *, void *, va_list);
typedef int (__cdecl *vws_t)(unsigned long long, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vs_t p_vs; static vws_t p_vws;
static int calls(char *b, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vs(0, b, 64, fmt, NULL, ap); va_end(ap); return r; }
static int callw(wchar_t *b, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vws(0, b, 64, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(pct_alone)
{
    char b[64]; int r;
    if (!p_vs) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    b[0] = 'S';
    r = calls(b, "%");
    /* observed on Win10 22H2 (all three cases): a format ending inside a conversion spec is silently truncated: no error, no errno, no handler */
    CHECK_EQ_I(r, 0); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(width_at_end)
{
    char b[64]; int r;
    if (!p_vs) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    r = calls(b, "ab%5");
    CHECK_EQ_I(r, 2); CHECK_STR(b, "ab"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_pct_alone)
{
    wchar_t b[64]; int r;
    if (!p_vws) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    b[0] = L'S';
    r = callw(b, L"%");
    CHECK_EQ_I(r, 0); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vs = (vs_t)tc_crt_proc("__stdio_common_vsprintf");
    p_vws = (vws_t)tc_crt_proc("__stdio_common_vswprintf");
    TC_RUN(pct_alone); TC_RUN(width_at_end); TC_RUN(wide_pct_alone);
    return tc_end();
}

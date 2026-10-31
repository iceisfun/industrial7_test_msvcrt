/* TC
 * id:    ucr_neg_stdio_vsprintf_unknown_conversion
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsprintf __stdio_common_vswprintf
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf format specification / Parameter validation (invalid format: handler, EINVAL, -1)
 * note:  PROBES: an unknown conversion character (%y) in the non-_s narrow/wide functions (options 0) is an invalid
 *        parameter: -1, EINVAL, handler once. The docs describe this for invalid format strings generally; the exact
 *        treatment of an unknown type character is not confirmed. The final valid-format case is a sanity control.
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

TC_CASE(narrow_y)
{
    char b[64]; int r;
    if (!p_vs) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    r = calls(b, "a%yb", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_y)
{
    wchar_t b[64]; int r;
    if (!p_vws) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    r = callw(b, L"a%yb", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(valid_still_ok)
{
    char b[64]; int r;
    if (!p_vs) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    r = calls(b, "a%db", 1);
    CHECK_EQ_I(r, 3); CHECK_STR(b, "a1b"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vs = (vs_t)tc_crt_proc("__stdio_common_vsprintf");
    p_vws = (vws_t)tc_crt_proc("__stdio_common_vswprintf");
    TC_RUN(narrow_y); TC_RUN(wide_y); TC_RUN(valid_still_ok);
    return tc_end();
}

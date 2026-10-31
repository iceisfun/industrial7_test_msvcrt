/* TC
 * id:    pfw_probe_stdio_common_wide_iso
 * area:  stdio_printf
 * funcs:
 * procs: __stdio_common_vswprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "Format specification syntax" / _CRT_STDIO_ISO_WIDE_SPECIFIERS
 * note:  Probing: with options==0 (no legacy-wide bit; what _CRT_STDIO_ISO_WIDE_SPECIFIERS selects) the wide function follows ISO C: %s takes char*, %ls wchar_t*.
 */
#include "tc.h"
#include <stdarg.h>
#include <stddef.h>

typedef int (__cdecl *vsw_t)(unsigned long long, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vsw_t p_vsw;
static int wf(wchar_t *b, size_t n, const wchar_t *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt); r = p_vsw(0, b, n, fmt, NULL, ap); va_end(ap);
    return r;
}
TC_CASE(iso_s_is_narrow) { wchar_t b[32]; int n; if (!p_vsw) TC_SKIP("no export"); n = wf(b, 32, L"[%s][%ls]", "nar", L"wid"); CHECK_WSTR(b, L"[nar][wid]"); CHECK_EQ_I(n, 10); }
int main(void)
{
    tc_begin(); p_vsw = (vsw_t)tc_crt_proc("__stdio_common_vswprintf");
    TC_RUN(iso_s_is_narrow); return tc_end();
}

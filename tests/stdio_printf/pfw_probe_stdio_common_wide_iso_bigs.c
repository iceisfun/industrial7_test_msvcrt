/* TC
 * id:    pfw_probe_stdio_common_wide_iso_bigs
 * area:  stdio_printf
 * funcs:
 * procs: __stdio_common_vswprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "Format specification syntax": %S is the opposite width of %s
 * note:  PROBING: with options==0 (ISO wide specifiers) in a wide function %S takes wchar_t* (opposite of %s=char*).
 *        Expected "[wide]". Unsure whether ISO mode keeps the MS 'opposite' meaning of %S.
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
TC_CASE(iso_S_is_wide) { wchar_t b[32]; int n; if (!p_vsw) TC_SKIP("no export"); n = wf(b, 32, L"[%S]", L"wide"); CHECK_WSTR(b, L"[wide]"); CHECK_EQ_I(n, 6); }
int main(void)
{
    tc_begin(); p_vsw = (vsw_t)tc_crt_proc("__stdio_common_vswprintf");
    TC_RUN(iso_S_is_wide); return tc_end();
}

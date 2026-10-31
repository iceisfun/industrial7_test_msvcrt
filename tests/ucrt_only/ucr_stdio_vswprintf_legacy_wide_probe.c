/* TC
 * id:    ucr_stdio_vswprintf_legacy_wide_probe
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vswprintf
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs Format specification syntax (_CRT_STDIO_ISO_WIDE_SPECIFIERS); corecrt_wstdio.h options
 * note:  PROBE: meaning of %s and %c in the wide printf core for options 0 (ISO: %s takes char*) versus
 *        0x4 _CRT_INTERNAL_PRINTF_LEGACY_WIDE_SPECIFIERS (MS legacy: %s takes wchar_t*, which is what the
 *        inline wprintf wrappers pass by default). Never passes a mismatched pointer type.
 */
#include "tc.h"
#include <stdarg.h>

typedef int (__cdecl *vsw_t)(unsigned long long, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vsw_t p_vsw;
static int callw(unsigned long long o, wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsw(o, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(opts0_s_narrow)
{
    wchar_t b[32];
    if (!p_vsw) TC_SKIP("not exported");
    callw(0, b, 32, L"%s", "abc");
    CHECK_WSTR(b, L"abc");
}
TC_CASE(opts4_s_wide)
{
    wchar_t b[32];
    if (!p_vsw) TC_SKIP("not exported");
    callw(4, b, 32, L"%s", L"abc");
    CHECK_WSTR(b, L"abc");
}
TC_CASE(opts0_c_narrow)
{
    wchar_t b[32];
    if (!p_vsw) TC_SKIP("not exported");
    callw(0, b, 32, L"%c", 'q');
    CHECK_WSTR(b, L"q");
}
TC_CASE(opts4_c_wide)
{
    wchar_t b[32];
    if (!p_vsw) TC_SKIP("not exported");
    callw(4, b, 32, L"%c", (wint_t)L'q');
    CHECK_WSTR(b, L"q");
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vsw = (vsw_t)tc_crt_proc("__stdio_common_vswprintf");
    TC_RUN(opts0_s_narrow); TC_RUN(opts4_s_wide); TC_RUN(opts0_c_narrow); TC_RUN(opts4_c_wide);
    return tc_end();
}

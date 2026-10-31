/* TC
 * id:    pfw_stdio_common_wide_spec_legacy
 * area:  stdio_printf
 * funcs:
 * procs: __stdio_common_vswprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   UCRT corecrt_stdio_config.h _CRT_INTERNAL_PRINTF_LEGACY_WIDE_SPECIFIERS (1<<2); MS docs "Format specification syntax" (s, S, C, hs, ls)
 * note:  With the legacy-wide-specifier option (the default set by the UCRT headers) %s in a wide function takes wchar_t*, %S takes char*, %c wide, %C narrow; %hs / %ls are unaffected.
 */
#include "tc.h"
#include <stdarg.h>
#include <stddef.h>

typedef int (__cdecl *vsw_t)(unsigned long long, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vsw_t p_vsw;
#define OPT_LEGACY_WIDE 4ULL

static int wf(wchar_t *b, size_t n, const wchar_t *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt); r = p_vsw(OPT_LEGACY_WIDE, b, n, fmt, NULL, ap); va_end(ap);
    return r;
}
TC_CASE(s_takes_wchar_ptr) { wchar_t b[32]; int n; if (!p_vsw) TC_SKIP("no export"); n = wf(b, 32, L"[%s]", L"wide"); CHECK_WSTR(b, L"[wide]"); CHECK_EQ_I(n, 6); }
TC_CASE(S_takes_char_ptr)  { wchar_t b[32]; int n; if (!p_vsw) TC_SKIP("no export"); n = wf(b, 32, L"[%S]", "narrow"); CHECK_WSTR(b, L"[narrow]"); CHECK_EQ_I(n, 8); }
TC_CASE(c_wide)
{
    wchar_t b[32]; int n; if (!p_vsw) TC_SKIP("no export");
    n = wf(b, 32, L"%c|%C", (int)0x4E2D, (int)'x');
    CHECK_EQ_I(n, 3); CHECK(b[0] == 0x4E2D && b[1] == L'|' && b[2] == L'x' && b[3] == 0);
}
TC_CASE(hs_ls_unaffected)
{
    wchar_t b[32]; int n; if (!p_vsw) TC_SKIP("no export");
    n = wf(b, 32, L"%hs|%ls", "nar", L"wid"); CHECK_WSTR(b, L"nar|wid"); CHECK_EQ_I(n, 7);
}
int main(void)
{
    tc_begin(); p_vsw = (vsw_t)tc_crt_proc("__stdio_common_vswprintf");
    TC_RUN(s_takes_wchar_ptr); TC_RUN(S_takes_char_ptr); TC_RUN(c_wide); TC_RUN(hs_ls_unaffected);
    return tc_end();
}

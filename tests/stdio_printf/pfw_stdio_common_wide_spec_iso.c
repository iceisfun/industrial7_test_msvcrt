/* TC
 * id:    pfw_stdio_common_wide_spec_iso
 * area:  stdio_printf
 * funcs:
 * procs: __stdio_common_vswprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   corecrt_stdio_config.h (_CRT_INTERNAL_PRINTF_LEGACY_WIDE_SPECIFIERS = 4), MS docs "Format specification syntax": with options=0 wide functions use ISO %s=char*, %ls=wchar_t* (%S moved to pfw_probe_stdio_common_wide_iso_bigs)
 */
#include "tc.h"
#include <stdarg.h>
#include <wchar.h>

typedef int (__cdecl *vsw_t)(unsigned long long, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vsw_t p_vsw;

static int call(unsigned long long opt, wchar_t *b, size_t n, const wchar_t *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt);
    r = p_vsw(opt, b, n, fmt, NULL, ap);
    va_end(ap);
    return r;
}

TC_CASE(exported) { CHECK(p_vsw != NULL); }
TC_CASE(s_takes_char_ptr) { wchar_t b[32]; volatile const char *s = "narrow"; int r; if (!p_vsw) TC_SKIP("missing"); r = call(0, b, 32, L"[%s]", (const char *)s); CHECK_WSTR(b, L"[narrow]"); CHECK_EQ_I(r, 8); }
TC_CASE(c_vs_C) { wchar_t b[32]; if (!p_vsw) TC_SKIP("missing"); call(0, b, 32, L"%c%C", (int)'A', (int)L'B'); CHECK_WSTR(b, L"AB"); }
TC_CASE(ls_and_hs) { wchar_t b[32]; if (!p_vsw) TC_SKIP("missing"); call(0, b, 32, L"%ls|%hs", L"w", "n"); CHECK_WSTR(b, L"w|n"); }

int main(void)
{
    tc_begin();
    p_vsw = (vsw_t)tc_crt_proc("__stdio_common_vswprintf");
    TC_RUN(exported); TC_RUN(s_takes_char_ptr); TC_RUN(c_vs_C); TC_RUN(ls_and_hs);
    return tc_end();
}

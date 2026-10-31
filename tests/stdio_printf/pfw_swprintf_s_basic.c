/* TC
 * id:    pfw_swprintf_s_basic
 * area:  stdio_printf
 * funcs: 
 * procs: swprintf_s vswprintf_s
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs sprintf_s, _sprintf_s_l, swprintf_s, _swprintf_s_l
 * note:  ucrtbase does not export swprintf_s (header inline over __stdio_common_vswprintf_s); old CRTs resolve from the DLL.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

typedef int (__cdecl *sws_t)(wchar_t *, size_t, const wchar_t *, ...);
typedef int (__cdecl *vsws_t)(wchar_t *, size_t, const wchar_t *, va_list);
static sws_t p_sws; static vsws_t p_vsws;
#if TC_CRT_VER >= 140
static int __cdecl shim_vsws(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return vswprintf_s(b, n, f, ap); }
static int __cdecl shim_sws(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vswprintf_s(b, n, f, ap); va_end(ap); return r; }
#endif

static void fill(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = L'Z'; }
static int callv(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsws(b, n, f, ap); va_end(ap); return r; }

TC_CASE(exported)
{
#if TC_CRT_VER >= 140
    CHECK(tc_crt_proc("__stdio_common_vswprintf_s") != NULL);
#else
    CHECK(p_sws != NULL); CHECK(p_vsws != NULL);
#endif
}
TC_CASE(basic_ret_len)
{
    wchar_t b[32]; int n; if (!p_sws) TC_SKIP("not exported");
    fill(b, 32); n = p_sws(b, 32, L"%d|%ls|%hs", -3, L"w", "n");
    CHECK_WSTR(b, L"-3|w|n"); CHECK_EQ_I(n, 6);
}
TC_CASE(exact_fit_len_size_minus_1)
{
    wchar_t b[16]; int n; if (!p_sws) TC_SKIP("not exported");
    fill(b, 16); n = p_sws(b, 6, L"%ls", L"abcde");
    CHECK_EQ_I(n, 5); CHECK_WSTR(b, L"abcde"); CHECK(b[5] == 0); CHECK(b[6] == L'Z');
}
TC_CASE(vswprintf_s)
{
    wchar_t b[32]; int n; if (!p_vsws) TC_SKIP("not exported");
    fill(b, 32); n = callv(b, 32, L"%04d-%ls", 7, L"x");
    CHECK_WSTR(b, L"0007-x"); CHECK_EQ_I(n, 6);
}
TC_CASE(empty_result_buf0_is_NUL)
{
    wchar_t b[8]; int n; if (!p_sws) TC_SKIP("not exported");
    fill(b, 8); n = p_sws(b, 8, L"");
    CHECK_EQ_I(n, 0); CHECK(b[0] == 0); CHECK(b[1] == L'Z');
}

int main(void)
{
    tc_begin();
#if TC_CRT_VER >= 140
    p_sws = shim_sws; p_vsws = shim_vsws;
#else
    p_sws = (sws_t)tc_crt_proc("swprintf_s"); p_vsws = (vsws_t)tc_crt_proc("vswprintf_s");
#endif
    TC_RUN(exported); TC_RUN(basic_ret_len); TC_RUN(exact_fit_len_size_minus_1); TC_RUN(vswprintf_s); TC_RUN(empty_result_buf0_is_NUL);
    return tc_end();
}

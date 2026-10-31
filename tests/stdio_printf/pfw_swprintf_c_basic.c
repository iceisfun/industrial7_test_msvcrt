/* TC
 * id:    pfw_swprintf_c_basic
 * area:  stdio_printf
 * funcs: 
 * procs: _swprintf_c _vswprintf_c
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs sprintf_c family: _sprintf_c, _swprintf_c, _vswprintf_c
 * note:  count-limited: writes at most count wchars including NUL when output fits; returns len. ucrtbase does not export these (header-inline over __stdio_common_vswprintf), so ucrt uses a shim with the header's documented behaviour; old CRTs resolve from the DLL. msvcrt.dll/msvcr80 export presence is checked by the exported case.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

typedef int (__cdecl *swc_t)(wchar_t *, size_t, const wchar_t *, ...);
typedef int (__cdecl *vswc_t)(wchar_t *, size_t, const wchar_t *, va_list);
static swc_t p_swc; static vswc_t p_vswc;
#if TC_CRT_VER >= 140
static int __cdecl shim_vswc(wchar_t *b, size_t n, const wchar_t *f, va_list ap)
{ int r = __stdio_common_vswprintf(_CRT_INTERNAL_LOCAL_PRINTF_OPTIONS, b, n, f, NULL, ap); return r < 0 ? -1 : r; }
static int __cdecl shim_swc(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = shim_vswc(b, n, f, ap); va_end(ap); return r; }
#endif

static void fill(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = L'Z'; }
static int callv(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vswc(b, n, f, ap); va_end(ap); return r; }

TC_CASE(exported)
{
#if TC_CRT_VER >= 140
    CHECK(tc_crt_proc("__stdio_common_vswprintf") != NULL);
#else
    CHECK(p_swc != NULL); CHECK(p_vswc != NULL);
#endif
}
TC_CASE(fits_len_and_NUL)
{
    wchar_t b[16]; int n; if (!p_swc) TC_SKIP("not exported");
    fill(b, 16); n = p_swc(b, 16, L"%d%ls", 12, L"ab");
    CHECK_EQ_I(n, 4); CHECK_WSTR(b, L"12ab"); CHECK(b[4] == 0);
}
TC_CASE(exact_len_eq_count_minus_1)
{
    wchar_t b[16]; int n; if (!p_swc) TC_SKIP("not exported");
    fill(b, 16); n = p_swc(b, 6, L"%ls", L"abcde");
    CHECK_EQ_I(n, 5); CHECK_WSTR(b, L"abcde"); CHECK(b[5] == 0); CHECK(b[6] == L'Z');
}
TC_CASE(vswprintf_c_variant)
{
    wchar_t b[16]; int n; if (!p_vswc) TC_SKIP("not exported");
    fill(b, 16); n = callv(b, 16, L"<%ls:%d>", L"k", 9);
    CHECK_EQ_I(n, 5); CHECK_WSTR(b, L"<k:9>"); CHECK(b[5] == 0);
}

int main(void)
{
    tc_begin();
#if TC_CRT_VER >= 140
    p_swc = shim_swc; p_vswc = shim_vswc;
#else
    p_swc = (swc_t)tc_crt_proc("_swprintf_c"); p_vswc = (vswc_t)tc_crt_proc("_vswprintf_c");
#endif
    TC_RUN(exported); TC_RUN(fits_len_and_NUL); TC_RUN(exact_len_eq_count_minus_1); TC_RUN(vswprintf_c_variant);
    return tc_end();
}

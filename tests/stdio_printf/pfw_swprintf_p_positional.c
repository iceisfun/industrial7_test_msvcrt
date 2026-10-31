/* TC
 * id:    pfw_swprintf_p_positional
 * area:  stdio_printf
 * funcs: 
 * procs: _swprintf_p _vswprintf_p
 * crt:   >=80
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs printf_p Positional Parameters; _sprintf_p, _swprintf_p, _vswprintf_p
 * note:  %n$ positional specifiers; ucrtbase header inline over __stdio_common_vswprintf_p. Star-width positional is in pfw_swprintf_p_star_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

typedef int (__cdecl *sp_t)(wchar_t *, size_t, const wchar_t *, ...);
typedef int (__cdecl *vsp_t)(wchar_t *, size_t, const wchar_t *, va_list);
static sp_t p_sp; static vsp_t p_vsp;
#if TC_CRT_VER >= 140
static int __cdecl shim_vsp(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return __stdio_common_vswprintf_p(_CRT_INTERNAL_LOCAL_PRINTF_OPTIONS, b, n, f, NULL, ap); }
static int __cdecl shim_sp(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf_p(_CRT_INTERNAL_LOCAL_PRINTF_OPTIONS, b, n, f, NULL, ap); va_end(ap); return r; }
#endif

static void fill(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = L'Z'; }
static int callv(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsp(b, n, f, ap); va_end(ap); return r; }

TC_CASE(exported)
{
#if TC_CRT_VER >= 140
    CHECK(tc_crt_proc("__stdio_common_vswprintf_p") != NULL);
#else
    CHECK(p_sp != NULL); CHECK(p_vsp != NULL);
#endif
}
TC_CASE(in_order)
{
    wchar_t b[32]; int n; if (!p_sp) TC_SKIP("not exported");
    fill(b, 32); n = p_sp(b, 32, L"%1$ls=%2$d", L"k", 5);
    CHECK_WSTR(b, L"k=5"); CHECK_EQ_I(n, 3);
}
TC_CASE(reorder_args)
{
    wchar_t b[32]; int n; if (!p_sp) TC_SKIP("not exported");
    fill(b, 32); n = p_sp(b, 32, L"%2$d %1$ls", L"name", 77);
    CHECK_WSTR(b, L"77 name"); CHECK_EQ_I(n, 7);
}
TC_CASE(repeat_arg)
{
    wchar_t b[32]; int n; if (!p_sp) TC_SKIP("not exported");
    fill(b, 32); n = p_sp(b, 32, L"%1$d-%1$d-%1$x", 255);
    CHECK_WSTR(b, L"255-255-ff"); CHECK_EQ_I(n, 10);
}
TC_CASE(vswprintf_p)
{
    wchar_t b[32]; int n; if (!p_vsp) TC_SKIP("not exported");
    fill(b, 32); n = callv(b, 32, L"%2$ls/%1$ls", L"a", L"b");
    CHECK_WSTR(b, L"b/a"); CHECK_EQ_I(n, 3);
}

int main(void)
{
    tc_begin(); tc_iph_install();
#if TC_CRT_VER >= 140
    p_sp = shim_sp; p_vsp = shim_vsp;
#else
    p_sp = (sp_t)tc_crt_proc("_swprintf_p"); p_vsp = (vsp_t)tc_crt_proc("_vswprintf_p");
#endif
    TC_RUN(exported); TC_RUN(in_order); TC_RUN(reorder_args); TC_RUN(repeat_arg); TC_RUN(vswprintf_p);
    return tc_end();
}

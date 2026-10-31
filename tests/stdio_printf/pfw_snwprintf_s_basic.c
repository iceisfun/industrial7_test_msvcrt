/* TC
 * id:    pfw_snwprintf_s_basic
 * area:  stdio_printf
 * funcs: 
 * procs: _snwprintf_s _vsnwprintf_s
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _snprintf_s, _snprintf_s_l, _snwprintf_s, _snwprintf_s_l; _vsnprintf_s, _vsnwprintf_s
 * note:  no-truncation paths only; truncation is covered by pfw_neg_snwprintf_s_truncate. ucrtbase: header inline.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

typedef int (__cdecl *sn_t)(wchar_t *, size_t, size_t, const wchar_t *, ...);
typedef int (__cdecl *vsn_t)(wchar_t *, size_t, size_t, const wchar_t *, va_list);
static sn_t p_sn; static vsn_t p_vsn;
#if TC_CRT_VER >= 140
static int __cdecl shim_vsn(wchar_t *b, size_t sz, size_t c, const wchar_t *f, va_list ap) { return _vsnwprintf_s(b, sz, c, f, ap); }
static int __cdecl shim_sn(wchar_t *b, size_t sz, size_t c, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnwprintf_s(b, sz, c, f, ap); va_end(ap); return r; }
#endif

static void fill(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = L'Z'; }
static int callv(wchar_t *b, size_t sz, size_t c, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsn(b, sz, c, f, ap); va_end(ap); return r; }

TC_CASE(exported)
{
#if TC_CRT_VER >= 140
    CHECK(tc_crt_proc("__stdio_common_vsnwprintf_s") != NULL);
#else
    CHECK(p_sn != NULL); CHECK(p_vsn != NULL);
#endif
}
TC_CASE(count_ge_len)
{
    wchar_t b[32]; int n; if (!p_sn) TC_SKIP("not exported");
    fill(b, 32); n = p_sn(b, 32, 31, L"%d%ls", 5, L"xy");
    CHECK_WSTR(b, L"5xy"); CHECK_EQ_I(n, 3); CHECK(b[3] == 0);
}
TC_CASE(count_equal_len)
{
    wchar_t b[32]; int n; if (!p_sn) TC_SKIP("not exported");
    fill(b, 32); n = p_sn(b, 32, 3, L"%ls", L"abc");
    CHECK_WSTR(b, L"abc"); CHECK_EQ_I(n, 3);
}
TC_CASE(vsnwprintf_s)
{
    wchar_t b[32]; int n; if (!p_vsn) TC_SKIP("not exported");
    fill(b, 32); n = callv(b, 32, 31, L"[%3d]", 9);
    CHECK_WSTR(b, L"[  9]"); CHECK_EQ_I(n, 5);
}
TC_CASE(size_equal_len_plus_1)
{
    wchar_t b[16]; int n; if (!p_sn) TC_SKIP("not exported");
    fill(b, 16); n = p_sn(b, 6, 5, L"%ls", L"abcde");
    CHECK_EQ_I(n, 5); CHECK_WSTR(b, L"abcde"); CHECK(b[5] == 0); CHECK(b[6] == L'Z');
}

int main(void)
{
    tc_begin();
#if TC_CRT_VER >= 140
    p_sn = shim_sn; p_vsn = shim_vsn;
#else
    p_sn = (sn_t)tc_crt_proc("_snwprintf_s"); p_vsn = (vsn_t)tc_crt_proc("_vsnwprintf_s");
#endif
    TC_RUN(exported); TC_RUN(count_ge_len); TC_RUN(count_equal_len); TC_RUN(vsnwprintf_s); TC_RUN(size_equal_len_plus_1);
    return tc_end();
}

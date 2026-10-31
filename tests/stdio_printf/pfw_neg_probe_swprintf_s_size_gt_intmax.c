/* TC
 * id:    pfw_neg_probe_swprintf_s_size_gt_intmax
 * area:  stdio_printf
 * funcs: _snwprintf
 * procs: vswprintf_s _vsnwprintf_s _vswprintf_s_l
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   none
 * note:  PROBE: sizeOfBuffer/count of 0x80000000 (> INT_MAX) with tiny valid output. Either the call succeeds (ret 1, buf "a") or it is rejected with -1; the test records both are acceptable but a rejection must set errno != 0, and nothing may crash. Legacy _snwprintf is likewise accepted either as 1 or -1 (x86 legacy CRTs scale count by sizeof(wchar_t), which could overflow).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>
#include <stdarg.h>
#if TC_CRT_VER >= 140
#define V_SW(b,n,f,ap)      vswprintf_s(b,n,f,ap)
#define V_SNW(b,n,c,f,ap)   _vsnwprintf_s(b,n,c,f,ap)
#define V_SWL(b,n,f,l,ap)   _vswprintf_s_l(b,n,f,l,ap)
#else
TC_DECL_PROC(int, vswprintf_s, (wchar_t *, size_t, const wchar_t *, va_list));
TC_DECL_PROC(int, _vsnwprintf_s, (wchar_t *, size_t, size_t, const wchar_t *, va_list));
TC_DECL_PROC(int, _vswprintf_s_l, (wchar_t *, size_t, const wchar_t *, void *, va_list));
#define V_SW(b,n,f,ap)      tc_p_vswprintf_s(b,n,f,ap)
#define V_SNW(b,n,c,f,ap)   tc_p__vsnwprintf_s(b,n,c,f,ap)
#define V_SWL(b,n,f,l,ap)   tc_p__vswprintf_s_l(b,n,f,l,ap)
#endif
static int w_swprintf_s(wchar_t *b, size_t n, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = V_SW(b, n, f, ap); va_end(ap); return r; }
static int w_snwprintf_s(wchar_t *b, size_t n, size_t c, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = V_SNW(b, n, c, f, ap); va_end(ap); return r; }
static int w_swprintf_s_l(wchar_t *b, size_t n, const wchar_t *f, void *l, ...) { va_list ap; int r; va_start(ap, l); r = V_SWL(b, n, f, l, ap); va_end(ap); return r; }

static const wchar_t *volatile fa = L"a";
static volatile size_t big = (size_t)0x80000000u;

TC_CASE(swprintf_s_big)
{
    wchar_t b[16] = { 9 }; int n; errno = 0; tc_iph_count = 0;
    n = w_swprintf_s(b, big, fa);
    if (n == 1) CHECK_WSTR(b, L"a"); else { CHECK_EQ_I(n, -1); CHECK(errno != 0); }
}
TC_CASE(snwprintf_s_big_count)
{
    wchar_t b[16] = { 9 }; int n; errno = 0; tc_iph_count = 0;
    n = w_snwprintf_s(b, 16, big, fa);
    if (n == 1) CHECK_WSTR(b, L"a"); else { CHECK_EQ_I(n, -1); CHECK(errno != 0); }
}
TC_CASE(legacy_snwprintf_big)
{
    wchar_t b[16] = { 9 }; int n = _snwprintf(b, big, fa);
    CHECK(n == 1 || n == -1);
}
int main(void)
{
    tc_begin(); tc_iph_install();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(vswprintf_s); TC_LOAD_PROC(_vsnwprintf_s); TC_LOAD_PROC(_vswprintf_s_l);
    CHECK(tc_p_vswprintf_s && tc_p__vsnwprintf_s && tc_p__vswprintf_s_l);
    if (!tc_p_vswprintf_s || !tc_p__vsnwprintf_s || !tc_p__vswprintf_s_l) return tc_end();
#endif
    TC_RUN(swprintf_s_big); TC_RUN(snwprintf_s_big_count); TC_RUN(legacy_snwprintf_big);
    return tc_end();
}

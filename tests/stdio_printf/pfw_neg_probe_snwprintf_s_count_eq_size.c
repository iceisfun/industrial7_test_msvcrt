/* TC
 * id:    pfw_neg_probe_snwprintf_s_count_eq_size
 * area:  stdio_printf
 * funcs:
 * procs: vswprintf_s _vsnwprintf_s _vswprintf_s_l
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _snprintf_s (count vs sizeOfBuffer)
 * note:  PROBE: _snwprintf_s(buf,4,4,L"abcd") (count == size == length, no room for NUL): expected -1 with ERANGE and handler, buf[0]==0. Also _snwprintf_s(buf,4,3,L"0123456789") count-truncation: expected -1 and buf == "012".
 */
#include "tc.h"
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

static const wchar_t *volatile f4 = L"abcd";
static const wchar_t *volatile f10 = L"0123456789";

TC_CASE(count_eq_size_exact)
{
    wchar_t b[4] = { 7, 7, 7, 7 }; int n; errno = 0; tc_iph_count = 0;
    n = w_snwprintf_s(b, 4, 4, f4);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(count_truncates)
{
    wchar_t b[4] = { 7, 7, 7, 7 }; int n; errno = 0; tc_iph_count = 0;
    n = w_snwprintf_s(b, 4, 3, f10);
    CHECK_EQ_I(n, -1); CHECK_WSTR(b, L"012");
}
int main(void)
{
    tc_begin(); tc_iph_install();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(vswprintf_s); TC_LOAD_PROC(_vsnwprintf_s); TC_LOAD_PROC(_vswprintf_s_l);
    CHECK(tc_p_vswprintf_s && tc_p__vsnwprintf_s && tc_p__vswprintf_s_l);
    if (!tc_p_vswprintf_s || !tc_p__vsnwprintf_s || !tc_p__vswprintf_s_l) return tc_end();
#endif
    TC_RUN(count_eq_size_exact); TC_RUN(count_truncates);
    return tc_end();
}

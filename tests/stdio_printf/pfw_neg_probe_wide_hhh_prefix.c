/* TC
 * id:    pfw_neg_probe_wide_hhh_prefix
 * area:  stdio_printf
 * funcs:
 * procs: vswprintf_s _vsnwprintf_s _vswprintf_s_l
 * crt:   >=140
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   none
 * note:  PROBE: "%hhhd" (triple h) in a wide _s format. Expected: -1/EINVAL/handler, buf[0]==0 (only h and hh are length modifiers). Pre-UCRT CRTs have no 'hh' and may treat it differently, so only ucrtbase is listed.
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

static const wchar_t *volatile f = L"%hhhd";

TC_CASE(hhh)
{
    wchar_t b[16]; int n, i; for (i = 0; i < 16; i++) b[i] = L'Z';
    errno = 0; tc_iph_count = 0;
    n = w_snwprintf_s(b, 16, (size_t)-1, f, 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(vswprintf_s); TC_LOAD_PROC(_vsnwprintf_s); TC_LOAD_PROC(_vswprintf_s_l);
    CHECK(tc_p_vswprintf_s && tc_p__vsnwprintf_s && tc_p__vswprintf_s_l);
    if (!tc_p_vswprintf_s || !tc_p__vsnwprintf_s || !tc_p__vswprintf_s_l) return tc_end();
#endif
    TC_RUN(hhh);
    return tc_end();
}

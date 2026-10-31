/* TC
 * id:    pfw_neg_probe_swprintf_s_invalid_conv_buf0
 * area:  stdio_printf
 * funcs:
 * procs: vswprintf_s _vsnwprintf_s _vswprintf_s_l
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * note:  PROBE: after swprintf_s fails on an invalid conversion (%y, trailing %) the buffer is set to an empty string (buf[0]==0). Documented for overflow/ERANGE but not explicitly for invalid format. For msvcr80-120 "%y" is replaced by incomplete "%5" (unknown-char handling there is uncertain).
 * ref:   MS docs sprintf_s, sprintf_s_l, swprintf_s, swprintf_s_l (invalid format string -> invalid parameter handler, EINVAL, buffer[0]=0)
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


#if TC_CRT_VER >= 140
static const wchar_t *volatile f_bad = L"ab%y";   /* unknown conversion char */
#else
/* msvcr80-120 may print an unknown conversion char literally; use a format cut off mid-spec instead (state != normal at end) */
static const wchar_t *volatile f_bad = L"ab%5";
#endif
static const wchar_t *volatile f_pct = L"abc%";
static const wchar_t *volatile f_ok = L"ok%d";

static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }

TC_CASE(bad_conv)
{
    wchar_t b[16]; int n; fill(b); errno = 0; tc_iph_count = 0;
    n = w_swprintf_s(b, 16, f_bad, 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(trailing_percent)
{
    wchar_t b[16]; int n; fill(b); errno = 0; tc_iph_count = 0;
    n = w_swprintf_s(b, 16, f_pct, 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(locale_variant)
{
    wchar_t b[16]; int n; fill(b); errno = 0; tc_iph_count = 0;
    n = w_swprintf_s_l(b, 16, f_bad, NULL, 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 0);
    fill(b); errno = 0; tc_iph_count = 0;
    n = w_swprintf_s_l(b, 16, f_pct, NULL, 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(handler_returns_then_valid)
{
    wchar_t b[16]; int n; tc_iph_count = 0;
    n = w_swprintf_s(b, 16, f_bad, 1);
    CHECK_EQ_I(n, -1);
    n = w_swprintf_s(b, 16, f_ok, 7);
    CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"ok7");
}
int main(void)
{
    tc_begin(); tc_iph_install();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(vswprintf_s); TC_LOAD_PROC(_vsnwprintf_s); TC_LOAD_PROC(_vswprintf_s_l);
    CHECK(tc_p_vswprintf_s && tc_p__vsnwprintf_s && tc_p__vswprintf_s_l);
    if (!tc_p_vswprintf_s || !tc_p__vsnwprintf_s || !tc_p__vswprintf_s_l) return tc_end();
#endif
    TC_RUN(bad_conv); TC_RUN(trailing_percent); TC_RUN(locale_variant); TC_RUN(handler_returns_then_valid);
    return tc_end();
}

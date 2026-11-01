/* TC
 * id:    pfw_neg_probe_wide_percent_z_unsupported
 * area:  stdio_printf
 * funcs:
 * procs: vswprintf_s _vsnwprintf_s _vswprintf_s_l
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs printf type field characters (no Z); 'w' length prefix documented
 * note:  "%Z" and "%wZ" (ntdll counted-string style) are not documented CRT conversions, but Win10 22H2 ucrtbase (observed) implements them: with a NULL argument both print "(null)" (6), errno 0, no handler (old guess: -1/EINVAL/handler). Pre-UCRT CRTs (msvcr80-120) historically implemented %Z (counted-string), so the %Z/%wZ cases are only run on ucrtbase and skipped elsewhere. Separately: "%wC" (w prefix on C) with L'x' expected to give 1 char "x".
 *        Raw evidence (baseline): pct_Z n == -1 got=6 want=-1 ; errno == EINVAL got=0 ; tc_iph_count == 1 got=0 (same for pct_wZ).
 *        crt narrowed to ucrtbase: the observed behaviour was measured on Win10 22H2 ucrtbase only (older CRTs [unverified]).
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

static const wchar_t *volatile fz = L"%Z";
static const wchar_t *volatile fwz = L"%wZ";
static const wchar_t *volatile fwc = L"%wC";

TC_CASE(pct_Z) { wchar_t b[32]; int n;
#if TC_CRT_VER < 140
    TC_SKIP("%Z may be implemented by pre-UCRT");
#endif
    errno = 0; tc_iph_count = 0; n = w_snwprintf_s(b, 32, (size_t)-1, fz, (void *)0);
    CHECK_EQ_I(n, 6); CHECK_WSTR(b, L"(null)"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(pct_wZ) { wchar_t b[32]; int n;
#if TC_CRT_VER < 140
    TC_SKIP("%wZ may be implemented by pre-UCRT");
#endif
    errno = 0; tc_iph_count = 0; n = w_snwprintf_s(b, 32, (size_t)-1, fwz, (void *)0);
    CHECK_EQ_I(n, 6); CHECK_WSTR(b, L"(null)"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(pct_wC) { wchar_t b[32]; int n; tc_iph_count = 0; n = w_snwprintf_s(b, 32, (size_t)-1, fwc, (int)L'x');
    CHECK_EQ_I(n, 1); CHECK_WSTR(b, L"x"); }
int main(void)
{
    tc_begin(); tc_iph_install();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(vswprintf_s); TC_LOAD_PROC(_vsnwprintf_s); TC_LOAD_PROC(_vswprintf_s_l);
    CHECK(tc_p_vswprintf_s && tc_p__vsnwprintf_s && tc_p__vswprintf_s_l);
    if (!tc_p_vswprintf_s || !tc_p__vsnwprintf_s || !tc_p__vswprintf_s_l) return tc_end();
#endif
    TC_RUN(pct_Z); TC_RUN(pct_wZ); TC_RUN(pct_wC);
    return tc_end();
}

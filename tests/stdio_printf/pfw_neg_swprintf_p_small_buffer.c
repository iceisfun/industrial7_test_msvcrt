/* TC
 * id:    pfw_neg_swprintf_p_small_buffer
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vswprintf_p
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _sprintf_p, _swprintf_p, _vswprintf_p: count-limited; too small a buffer returns a negative value; never writes more than count chars
 * note:  On ucrtbase _swprintf_p is inline over __stdio_common_vswprintf_p (options 4); legacy CRTs resolve _vswprintf_p at run time.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
static int sh_vp(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return __stdio_common_vswprintf_p(4, b, n, f, NULL, ap); }
#else
typedef int (__cdecl *vp_t)(wchar_t *, size_t, const wchar_t *, va_list);
static vp_t p_vp;
static int sh_vp(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return p_vp ? p_vp(b, n, f, ap) : -99; }
#endif
static int swp(wchar_t *b, size_t n, const wchar_t *f, ...) { int r; va_list ap; va_start(ap, f); r = sh_vp(b, n, f, ap); va_end(ap); return r; }
static const wchar_t * volatile pfmt = L"%1$s%2$s";
static const wchar_t * volatile rfmt = L"%2$s%1$s";
static const wchar_t * volatile sa = L"abcde";
static const wchar_t * volatile sb = L"fghij";
static const wchar_t * volatile sc = L"ab";
static const wchar_t * volatile sd = L"cdef";
static void fill(wchar_t *b) { int i; for (i = 0; i < 8; i++) b[i] = L'Q'; }

TC_CASE(too_small)
{
    wchar_t b[8]; int r; int i; fill(b);
    r = swp(b, 4, pfmt, sa, sb);
    CHECK(r < 0);
    for (i = 4; i < 8; i++) CHECK_EQ_I(b[i], L'Q');
}
TC_CASE(exact_fit)
{
    wchar_t b[8]; int r; fill(b);
    r = swp(b, 7, rfmt, sc, sd);
    CHECK_EQ_I(r, 6); CHECK_WSTR(b, L"cdefab"); CHECK_EQ_I(b[7], L'Q');
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
#if TC_CRT_VER < 140
    p_vp = (vp_t)tc_crt_proc("_vswprintf_p"); CHECK(p_vp != NULL);
#endif
    TC_RUN(too_small); TC_RUN(exact_fit);
    return tc_end();
}

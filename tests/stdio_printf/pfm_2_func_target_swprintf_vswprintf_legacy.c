/* TC
 * id:    pfm_2_func_target_swprintf_vswprintf_legacy
 * area:  stdio_printf
 * funcs:
 * procs: swprintf _swprintf vswprintf _vswprintf _swprintf_c _vswprintf_c
 * crt:   <140
 * tier:  p1
 * conf:  documented
 * ref:   MS docs swprintf, _swprintf, vswprintf (pre-UCRT: no count parameter; swprintf(buffer, format, ...) returns chars written, excluding NUL); _swprintf_c, _vswprintf_c (count form)
 * note:  msvcr80+ export the no-count forms under the underscore names _swprintf/_vswprintf; msvcrt.dll as swprintf/vswprintf.
 *        The pre-ucrt DLL exports of swprintf/vswprintf are the non-conforming no-count forms, so they are called through procs with
 *        hand-written prototypes (mingw headers vary the prototype by __MSVCRT_VERSION__). _swprintf_c may be absent on msvcrt.dll: skipped then.
 */
#include "tc.h"
#include <stdarg.h>
#include <stddef.h>

typedef int (__cdecl *sw_t)(wchar_t *, const wchar_t *, ...);
typedef int (__cdecl *vsw_t)(wchar_t *, const wchar_t *, va_list);
typedef int (__cdecl *swc_t)(wchar_t *, size_t, const wchar_t *, ...);
typedef int (__cdecl *vswc_t)(wchar_t *, size_t, const wchar_t *, va_list);
static sw_t p_sw; static vsw_t p_vsw; static swc_t p_swc; static vswc_t p_vswc;

static int callv(wchar_t *b, const wchar_t *f, ...) { int r; va_list ap; va_start(ap, f); r = p_vsw(b, f, ap); va_end(ap); return r; }
static int callvc(wchar_t *b, size_t n, const wchar_t *f, ...) { int r; va_list ap; va_start(ap, f); r = p_vswc(b, n, f, ap); va_end(ap); return r; }

TC_CASE(exported) { CHECK(p_sw != NULL); CHECK(p_vsw != NULL); }
TC_CASE(swprintf_no_count)
{
    wchar_t b[32]; const wchar_t * volatile f = L"%d"; int n;
    if (!p_sw) TC_SKIP("swprintf absent");
    n = p_sw(b, f, 5);
    CHECK_EQ_I(n, 1); CHECK_WSTR(b, L"5");
    n = p_sw(b, L"%ls-%d|%x", L"abc", -12, 255);
    CHECK_EQ_I(n, 10); CHECK_WSTR(b, L"abc--12|ff");
}
TC_CASE(vswprintf_no_count)
{
    wchar_t b[32]; int n;
    if (!p_vsw) TC_SKIP("vswprintf absent");
    n = callv(b, L"[%5d][%-3d]", 42, 7);
    CHECK_EQ_I(n, 12); CHECK_WSTR(b, L"[   42][7  ]");
}
TC_CASE(count_form_c)
{
    wchar_t b[32]; int n;
    if (!p_swc || !p_vswc) TC_SKIP("_swprintf_c/_vswprintf_c absent");
    n = p_swc(b, 32, L"%d", 5);
    CHECK_EQ_I(n, 1); CHECK_WSTR(b, L"5");
    n = callvc(b, 32, L"%ls", L"xyz");
    CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"xyz");
}
int main(void)
{
    tc_begin();
    /* msvcrt.dll exports swprintf/vswprintf; msvcr80+ export the no-count forms as _swprintf/_vswprintf (headers map swprintf to them) */
    p_sw = (sw_t)tc_crt_proc(TC_AT_LEAST(80) ? "_swprintf" : "swprintf"); p_vsw = (vsw_t)tc_crt_proc(TC_AT_LEAST(80) ? "_vswprintf" : "vswprintf");
    p_swc = (swc_t)tc_crt_proc("_swprintf_c"); p_vswc = (vswc_t)tc_crt_proc("_vswprintf_c");
    TC_RUN(exported); TC_RUN(swprintf_no_count); TC_RUN(vswprintf_no_count); TC_RUN(count_form_c);
    return tc_end();
}

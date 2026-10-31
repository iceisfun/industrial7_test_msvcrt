/* TC
 * id:    pfw_neg_vswprintf_c_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vswprintf_c
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _swprintf_c, _vswprintf_c: NULL buffer/format -> invalid parameter handler, -1, EINVAL
 * note:  On ucrtbase _vswprintf_c is an inline wrapper over __stdio_common_vswprintf (options 4); legacy CRTs resolve the exports at run time.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
static int sh_vc(wchar_t *b, size_t n, const wchar_t *f, _locale_t l, va_list ap) { return __stdio_common_vswprintf(4, b, n, f, l, ap); }
#else
typedef int (__cdecl *vc_t)(wchar_t *, size_t, const wchar_t *, va_list);
static vc_t p_vc;
static int sh_vc(wchar_t *b, size_t n, const wchar_t *f, _locale_t l, va_list ap)
{ (void)l; return p_vc ? p_vc(b, n, f, ap) : -99; }
#endif
static int vswc(wchar_t *b, size_t n, const wchar_t *f, ...) { int r; va_list ap; va_start(ap, f); r = sh_vc(b, n, f, NULL, ap); va_end(ap); return r; }
static wchar_t * volatile nbuf;
static const wchar_t * volatile nfmt;
static const wchar_t * volatile afmt = L"a";

TC_CASE(null_buffer) { int r; tc_iph_count = 0; errno = 0; r = vswc(nbuf, 4, afmt); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_format) { wchar_t b[4] = { L'Q', 0 }; int r; tc_iph_count = 0; errno = 0; r = vswc(b, 4, nfmt); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(control) { wchar_t b[4]; tc_iph_count = 0; CHECK_EQ_I(vswc(b, 4, afmt), 1); CHECK_WSTR(b, L"a"); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
#if TC_CRT_VER < 140
    p_vc = (vc_t)tc_crt_proc("_vswprintf_c");
    CHECK(p_vc != NULL);
#endif
    TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(control);
    return tc_end();
}

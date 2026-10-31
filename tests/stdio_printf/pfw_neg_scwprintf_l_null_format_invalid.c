/* TC
 * id:    pfw_neg_scwprintf_l_null_format_invalid
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vscwprintf_l
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _scprintf_l, _scwprintf_l, _vscwprintf_l: NULL format -> invalid parameter handler, -1, EINVAL; NULL locale means current locale
 * note:  _vscwprintf_l is resolved at run time on legacy CRTs; on ucrtbase it is inline over __stdio_common_vswprintf
 *        (options 6, NULL buffer/0). The ucrt "_scwprintf_l" is the same thing with an ellipsis.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
static int sh_vscwl(const wchar_t *f, _locale_t l, va_list ap) { return __stdio_common_vswprintf(6, NULL, 0, f, l, ap); }
#else
typedef int (__cdecl *vscwl_t)(const wchar_t *, _locale_t, va_list);
static vscwl_t p_vscwprintf_l;
static int sh_vscwl(const wchar_t *f, _locale_t l, va_list ap) { return p_vscwprintf_l ? p_vscwprintf_l(f, l, ap) : -99; }
#endif
static int scwl(const wchar_t *f, _locale_t l, ...) { int r; va_list ap; va_start(ap, l); r = sh_vscwl(f, l, ap); va_end(ap); return r; }
static int vscwl_wrap(const wchar_t *f, _locale_t l, ...) { return scwl(f, l, 1234); }
static const wchar_t * volatile nfmt;
static const wchar_t * volatile dfmt = L"%d";
static _locale_t volatile nloc;

TC_CASE(null_format) { int r; tc_iph_count = 0; errno = 0; r = scwl(nfmt, nloc); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_format_args) { int r; tc_iph_count = 0; errno = 0; r = scwl(nfmt, nloc, 1, 2); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(control_null_locale) { int r; tc_iph_count = 0; errno = 0; r = scwl(dfmt, nloc, 7); CHECK_EQ_I(r, 1); CHECK_EQ_I(vscwl_wrap(dfmt, nloc), 4); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
#if TC_CRT_VER < 140
    p_vscwprintf_l = (vscwl_t)tc_crt_proc("_vscwprintf_l");
    CHECK(p_vscwprintf_l != NULL);
#endif
    TC_RUN(null_format); TC_RUN(null_format_args); TC_RUN(control_null_locale);
    return tc_end();
}

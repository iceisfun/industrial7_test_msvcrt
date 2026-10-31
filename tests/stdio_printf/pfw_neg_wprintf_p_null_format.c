/* TC
 * id:    pfw_neg_wprintf_p_null_format
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vwprintf_p
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _printf_p, _wprintf_p, _vwprintf_p: NULL format -> invalid parameter handler, -1, EINVAL
 * note:  Only failing calls are made, so nothing is written to stdout. On ucrtbase _wprintf_p is inline over
 *        __stdio_common_vfwprintf_p (options 4, stdout); legacy CRTs resolve _vwprintf_p at run time.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
static int sh_vw(const wchar_t *f, va_list ap) { return __stdio_common_vfwprintf_p(4, stdout, f, NULL, ap); }
#else
typedef int (__cdecl *vw_t)(const wchar_t *, va_list);
static vw_t p_vw;
static int sh_vw(const wchar_t *f, va_list ap) { return p_vw ? p_vw(f, ap) : -99; }
#endif
static int wp(const wchar_t *f, ...) { int r; va_list ap; va_start(ap, f); r = sh_vw(f, ap); va_end(ap); return r; }
static const wchar_t * volatile nfmt;

TC_CASE(null_format) { int r; tc_iph_count = 0; errno = 0; r = wp(nfmt); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
#if TC_CRT_VER < 140
    p_vw = (vw_t)tc_crt_proc("_vwprintf_p"); CHECK(p_vw != NULL);
#endif
    TC_RUN(null_format);
    return tc_end();
}

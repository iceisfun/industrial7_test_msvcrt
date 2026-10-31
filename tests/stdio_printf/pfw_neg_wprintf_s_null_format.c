/* TC
 * id:    pfw_neg_wprintf_s_null_format
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: vwprintf_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_s, wprintf_s, vwprintf_s: NULL format -> invalid parameter handler, -1, EINVAL; %n not supported (disabled by default, _set_printf_count_output)
 * note:  Only failing calls are made, so nothing is written to stdout. On ucrtbase wprintf_s is inline over
 *        __stdio_common_vfwprintf_s (options 4, stdout); legacy CRTs resolve vwprintf_s at run time.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
extern int __cdecl __stdio_common_vfwprintf_s(unsigned __int64, FILE *, const wchar_t *, _locale_t, va_list);
static int sh_vw(const wchar_t *f, va_list ap) { return __stdio_common_vfwprintf_s(4, stdout, f, NULL, ap); }
#else
typedef int (__cdecl *vw_t)(const wchar_t *, va_list);
static vw_t p_vw;
static int sh_vw(const wchar_t *f, va_list ap) { return p_vw ? p_vw(f, ap) : -99; }
#endif
static int wps(const wchar_t *f, ...) { int r; va_list ap; va_start(ap, f); r = sh_vw(f, ap); va_end(ap); return r; }
static const wchar_t * volatile nfmt;
static const wchar_t * volatile nfmt_n = L"%n";

TC_CASE(null_format) { int r; tc_iph_count = 0; errno = 0; r = wps(nfmt); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(percent_n_disabled) { int x = 12345; int r; tc_iph_count = 0; errno = 0; r = wps(nfmt_n, &x); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(x, 12345); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
#if TC_CRT_VER < 140
    p_vw = (vw_t)tc_crt_proc("vwprintf_s"); CHECK(p_vw != NULL);
#endif
    TC_RUN(null_format); TC_RUN(percent_n_disabled);
    return tc_end();
}

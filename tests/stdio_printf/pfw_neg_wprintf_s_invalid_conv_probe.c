/* TC
 * id:    pfw_neg_wprintf_s_invalid_conv_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: vwprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf_s: invalid formatting characters -> invalid parameter handler; UCRT validates conversion characters
 * note:  PROBE. wprintf_s with an unknown conversion (%y) is probed to fail with -1/EINVAL via the handler on ucrtbase. Older CRTs may print
 *        the character literally, hence ucrtbase only. Nothing is written to stdout.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
extern int __cdecl __stdio_common_vfwprintf_s(unsigned __int64, FILE *, const wchar_t *, _locale_t, va_list);
static int sh_vw(const wchar_t *f, va_list ap) { return __stdio_common_vfwprintf_s(4, stdout, f, NULL, ap); }
#endif
static int wps(const wchar_t *f, ...) { int r; va_list ap; va_start(ap, f); r = sh_vw(f, ap); va_end(ap); return r; }
static const wchar_t * volatile yfmt = L"%y";

TC_CASE(invalid_conversion) { int r; tc_iph_count = 0; errno = 0; r = wps(yfmt); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(invalid_conversion);
    return tc_end();
}

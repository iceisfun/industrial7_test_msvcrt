/* TC
 * id:    pfo_neg_vsprintf_s_percent_n_wide_and_p
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vswprintf_s __stdio_common_vsprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_s, wprintf_s ("The %n format specifier is not supported" in the secure versions: invalid parameter handler); MS docs _set_printf_count_output (%n disabled by default)
 * note:  The %n target must never be written when the call is rejected. (%n through the _p/positional entry is a probe:
 *        see pfo_neg_probe_vsprintf_p_percent_n_positional.)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <errno.h>

#define SENT 0x5a5a5a5a

static int cws(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int cs(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(wide_s_rejects_n)
{
    wchar_t b[16]; int t = SENT, r;
    tc_iph_count = 0; errno = 0;
    r = cws(b, 16, L"ab%n", &t);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(t, SENT); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(narrow_s_after_args)
{
    char b[32]; int t = SENT, r;
    tc_iph_count = 0; errno = 0;
    r = cs(b, sizeof b, "x%dy%n", 5, &t);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(t, SENT); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(wide_s_rejects_n); TC_RUN(narrow_s_after_args);
    return tc_end();
}

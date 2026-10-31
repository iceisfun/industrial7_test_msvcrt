/* TC
 * id:    pfo_neg_probe_vsprintf_positional_nonp
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf __stdio_common_vsprintf_s __stdio_common_vsprintf_p __stdio_common_vswprintf __stdio_common_vswprintf_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf_p positional parameters (only the _p functions support "%n$")
 * note:  PROBES: the non-_p workers treat "%1$d" as an invalid format (negative return; the _s workers also call the invalid parameter
 *        handler and set EINVAL). Control: the _p worker accepts the same format (documented).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

int __cdecl __stdio_common_vsprintf_s(unsigned __int64, char *, size_t, const char *, _locale_t, va_list);
int __cdecl __stdio_common_vswprintf_s(unsigned __int64, wchar_t *, size_t, const wchar_t *, _locale_t, va_list);

static int cv(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int cs(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int cp(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_p(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int cw(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int cws(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }
static const char * volatile fmt = "%1$d";
static const wchar_t * volatile wfmt = L"%1$d";

TC_CASE(control_p) { char b[16]; int r = cp(b, 16, fmt, 5); CHECK_EQ_I(r, 1); CHECK_STR(b, "5"); }
TC_CASE(nonp_narrow) { char b[16]; int r; errno = 0; r = cv(b, 16, fmt, 5); CHECK(r < 0); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(s_narrow) { char b[16]; int r; tc_iph_count = 0; errno = 0; r = cs(b, 16, fmt, 5); CHECK(r < 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(nonp_wide) { wchar_t b[16]; int r; errno = 0; r = cw(b, 16, wfmt, 5); CHECK(r < 0); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(s_wide) { wchar_t b[16]; int r; tc_iph_count = 0; errno = 0; r = cws(b, 16, wfmt, 5); CHECK(r < 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(control_p); TC_RUN(nonp_narrow); TC_RUN(s_narrow); TC_RUN(nonp_wide); TC_RUN(s_wide);
    return tc_end();
}

/* TC
 * id:    pfo_neg_probe_vsprintf_legacy_trunc_errno
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf __stdio_common_vswprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _vsnprintf, vswprintf
 * note:  PROBES: truncation via the non-_s entries (options 0 and 1, narrow and wide) is expected NOT to invoke the invalid-parameter handler
 *        and not to set EINVAL (errno stays 0 or becomes ERANGE); with options 2 (C99) errno is expected to stay unchanged.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <errno.h>

static int c0(unsigned o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static int cw(unsigned o, wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(narrow_opt0) { char b[16]; int r; tc_iph_count = 0; errno = 0; r = c0(0, b, 4, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK(errno == 0 || errno == ERANGE); }
TC_CASE(wide_opt0) { wchar_t b[16]; int r; tc_iph_count = 0; errno = 0; r = cw(0, b, 4, L"%ls", L"abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK(errno == 0 || errno == ERANGE); }
TC_CASE(narrow_opt1) { char b[16]; int r; tc_iph_count = 0; errno = 0; r = c0(1, b, 4, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK(errno == 0 || errno == ERANGE); }
TC_CASE(narrow_opt2_errno_kept) { char b[16]; int r; tc_iph_count = 0; errno = 12345; r = c0(2, b, 4, "%s", "abcdefgh");
    CHECK_EQ_I(r, 8); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 12345); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(narrow_opt0); TC_RUN(wide_opt0); TC_RUN(narrow_opt1); TC_RUN(narrow_opt2_errno_kept);
    return tc_end();
}

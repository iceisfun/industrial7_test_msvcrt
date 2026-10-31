/* TC
 * id:    pfo_neg_probe_vsprintf_ls_invalid_wide_char
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf __stdio_common_vsprintf_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf Type field (%lc, %ls convert via wctomb in the current locale); wcrtomb EILSEQ
 * note:  PROBES: in the "C" locale U+4E2D has no single-byte mapping, so %lc / %ls conversion is expected to fail with -1 and EILSEQ;
 *        the secure entry is expected to report it without invoking the invalid-parameter handler.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int c0(unsigned o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static int cs(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(lc_opt0) { char b[16]; int r; errno = 0; r = c0(0, b, sizeof b, "%lc", (wint_t)0x4E2D); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ); }
TC_CASE(ls_opt0) { char b[16]; int r; errno = 0; r = c0(0, b, sizeof b, "%ls", L"a\x4E2D"); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ); }
TC_CASE(lc_secure) { char b[16]; int r; errno = 0; tc_iph_count = 0; r = cs(b, sizeof b, "%lc", (wint_t)0x4E2D);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(lc_legacy_wide_opt) { char b[16]; int r; errno = 0; r = c0(4, b, sizeof b, "%lc", (wint_t)0x4E2D); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(lc_opt0); TC_RUN(ls_opt0); TC_RUN(lc_secure); TC_RUN(lc_legacy_wide_opt);
    return tc_end();
}

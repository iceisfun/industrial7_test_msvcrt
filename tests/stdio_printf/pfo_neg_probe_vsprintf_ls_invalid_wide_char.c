/* TC
 * id:    pfo_neg_probe_vsprintf_ls_invalid_wide_char
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf __stdio_common_vsprintf_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs printf Type field (%lc, %ls convert via wctomb in the current locale); wcrtomb EILSEQ
 * note:  Observed on Windows 10 22H2, "C" locale, U+4E2D: %ls fails with -1/EILSEQ, but %lc does NOT fail: it emits nothing, sets errno to EILSEQ
 *        and the call returns 0 (a successful empty result), also through the secure entry (no invalid-parameter handler call). Deviates from
 *        C11 7.21.6.1p8 (%lc is converted as by wcrtomb) and 7.21.6.1p14 (an encoding error makes the function return a negative value).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int c0(unsigned o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static int cs(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(lc_opt0) { char b[16]; int r; errno = 0; r = c0(0, b, sizeof b, "%lc", (wint_t)0x4E2D); CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EILSEQ); CHECK_EQ_I(b[0], 0); }
TC_CASE(ls_opt0) { char b[16]; int r; errno = 0; r = c0(0, b, sizeof b, "%ls", L"a\x4E2D"); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ); }
TC_CASE(lc_secure) { char b[16]; int r; errno = 0; tc_iph_count = 0; r = cs(b, sizeof b, "%lc", (wint_t)0x4E2D);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EILSEQ); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(b[0], 0); }
TC_CASE(lc_legacy_wide_opt) { char b[16]; int r; errno = 0; r = c0(4, b, sizeof b, "%lc", (wint_t)0x4E2D); CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EILSEQ); CHECK_EQ_I(b[0], 0); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(lc_opt0); TC_RUN(ls_opt0); TC_RUN(lc_secure); TC_RUN(lc_legacy_wide_opt);
    return tc_end();
}

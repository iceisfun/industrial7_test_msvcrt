/* TC
 * id:    pfo_neg_vsnprintf_s_wide_null_and_zero
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsnwprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s, _snwprintf_s, _vsnwprintf_s: NULL format -> invalid parameter handler, EINVAL; count > size without _TRUNCATE -> buffer[0]=0, ERANGE; _TRUNCATE -> truncation, returns -1
 * note:  Direct calls to the UCRT worker (options 0). Size 0 with a non-NULL buffer is probed in pfo_neg_probe_vsnprintf_s_zero_cases.
 *        Overflow paths assert r < 0 only (raw worker may return a negative value other than -1).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

int __cdecl __stdio_common_vsnwprintf_s(unsigned __int64, wchar_t *, size_t, size_t, const wchar_t *, _locale_t, va_list);
static int cn(wchar_t *b, size_t n, size_t c, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsnwprintf_s(0, b, n, c, f, NULL, ap); va_end(ap); return r; }
static const wchar_t * volatile nfmt;
static volatile size_t sz3 = 3;

TC_CASE(null_format) { wchar_t b[8]; int r; tc_iph_count = 0; errno = 0; r = cn(b, 8, 8, nfmt); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(count_over_size)
{
    wchar_t b[8]; int r, i; for (i = 0; i < 8; i++) b[i] = L'Z';
    tc_iph_count = 0; errno = 0; r = cn(b, sz3, 10, L"abcdef");
    CHECK(r < 0); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
    for (i = 3; i < 8; i++) CHECK_EQ_I(b[i], L'Z');
}
TC_CASE(truncate)
{
    wchar_t b[8]; int r; tc_iph_count = 0;
    r = cn(b, sz3, (size_t)-1, L"abcdef");
    CHECK(r < 0); CHECK_WSTR(b, L"ab"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_format); TC_RUN(count_over_size); TC_RUN(truncate);
    return tc_end();
}

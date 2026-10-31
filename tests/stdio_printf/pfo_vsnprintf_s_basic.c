/* TC
 * id:    pfo_vsnprintf_s_basic
 * area:  stdio_printf
 * funcs: __stdio_common_vsnprintf_s __stdio_common_vsnwprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s, _snwprintf_s ("_TRUNCATE: write as much as fits, NUL-terminate, return -1, no invalid parameter handler")
 * note:  _TRUNCATE is (size_t)-1. The count < len case (count != _TRUNCATE) is covered by pfo_neg_vsnprintf_s_count.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

#define TRUNC ((size_t)-1)
static int cn(char *b, size_t n, size_t c, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = __stdio_common_vsnprintf_s(0, b, n, c, f, NULL, ap); va_end(ap); return r; }
static int cw(wchar_t *b, size_t n, size_t c, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = __stdio_common_vsnwprintf_s(0, b, n, c, f, NULL, ap); va_end(ap); return r; }

TC_CASE(truncate)
{
    char b[16]; int i; volatile size_t n = 4; int r; for (i = 0; i < 16; i++) b[i] = 'Z';
    tc_iph_count = 0;
    r = cn(b, n, TRUNC, "%s", "abcdefg");
    CHECK_EQ_I(r, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(b[4], 'Z'); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(fits) { char b[16]; int r = cn(b, 16, TRUNC, "%s-%d", "ab", 9); CHECK_EQ_I(r, 4); CHECK_STR(b, "ab-9"); }
TC_CASE(exact_fit_trunc) { char b[16]; volatile size_t n = 5; int r = cn(b, n, TRUNC, "%s", "abcd"); CHECK_EQ_I(r, 4); CHECK_STR(b, "abcd"); }
TC_CASE(wide_truncate)
{
    wchar_t b[16]; int i; volatile size_t n = 4; int r; for (i = 0; i < 16; i++) b[i] = L'Z';
    tc_iph_count = 0;
    r = cw(b, n, TRUNC, L"%ls", L"abcdefg");
    CHECK_EQ_I(r, -1); CHECK_WSTR(b, L"abc"); CHECK_EQ_I(b[4], L'Z'); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_fits) { wchar_t b[16]; int r = cw(b, 16, TRUNC, L"%ls-%d", L"ab", 9); CHECK_EQ_I(r, 4); CHECK_WSTR(b, L"ab-9"); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(truncate); TC_RUN(fits); TC_RUN(exact_fit_trunc); TC_RUN(wide_truncate); TC_RUN(wide_fits);
    return tc_end();
}

/* TC
 * id:    pfu_vsnprintf_s_basic
 * area:  stdio_printf
 * funcs: _vsnprintf_s _vsnwprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _vsnprintf_s, _vsnprintf_s_l, _vsnwprintf_s: count semantics
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
static int cv(char *b, size_t n, size_t c, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnprintf_s(b, n, c, f, ap); va_end(ap); return r; }
static int cw(wchar_t *b, size_t n, size_t c, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnwprintf_s(b, n, c, f, ap); va_end(ap); return r; }

TC_CASE(count_ge_len)
{
    char b[16]; volatile size_t n = 16, c = 10; int r;
    r = cv(b, n, c, "%d%s", 12, "345");
    CHECK_EQ_I(r, 5); CHECK_STR(b, "12345");
}
TC_CASE(count_lt_len)
{
    char b[16]; volatile size_t n = 16, c = 3; int r;
    tc_iph_count = 0;
    r = cv(b, n, c, "%s", "abcdef");
    CHECK_EQ_I(r, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(truncate)
{
    char b[16]; volatile size_t n = 5, c = _TRUNCATE; int r;
    tc_iph_count = 0;
    r = cv(b, n, c, "%s", "abcdef");
    CHECK_EQ_I(r, -1); CHECK_STR(b, "abcd"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_count_ge_len)
{
    wchar_t w[16]; volatile size_t n = 16, c = 10; int r;
    r = cw(w, n, c, L"%d%ls", 12, L"345");
    CHECK_EQ_I(r, 5); CHECK_WSTR(w, L"12345");
}
TC_CASE(wide_count_lt_len)
{
    wchar_t w[16]; volatile size_t n = 16, c = 3; int r;
    tc_iph_count = 0;
    r = cw(w, n, c, L"%ls", L"abcdef");
    CHECK_EQ_I(r, -1); CHECK_WSTR(w, L"abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_truncate)
{
    wchar_t w[16]; volatile size_t n = 5, c = _TRUNCATE; int r;
    tc_iph_count = 0;
    r = cw(w, n, c, L"%ls", L"abcdef");
    CHECK_EQ_I(r, -1); CHECK_WSTR(w, L"abcd"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(count_ge_len); TC_RUN(count_lt_len); TC_RUN(truncate);
    TC_RUN(wide_count_ge_len); TC_RUN(wide_count_lt_len); TC_RUN(wide_truncate);
    return tc_end();
}

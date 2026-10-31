/* TC
 * id:    pfu_vsprintf_s_basic
 * area:  stdio_printf
 * funcs:
 * procs: vsprintf_s vswprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vsprintf_s, vsprintf_s_l, vswprintf_s
 * note:  resolved at run time (procs) for the legacy DLLs because mingw's static versions would otherwise be linked for msvcrt.dll. ucrtbase does not export vsprintf_s/vswprintf_s (they are inline in the UCRT headers over __stdio_common_*), so the header inline version is called there.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

#ifdef _UCRT
#define tc_p_vsprintf_s vsprintf_s
#define tc_p_vswprintf_s vswprintf_s
#else
TC_DECL_PROC(int, vsprintf_s, (char *, size_t, const char *, va_list));
TC_DECL_PROC(int, vswprintf_s, (wchar_t *, size_t, const wchar_t *, va_list));
#endif
static int cv(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = tc_p_vsprintf_s(b, n, f, ap); va_end(ap); return r; }
static int cw(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = tc_p_vswprintf_s(b, n, f, ap); va_end(ap); return r; }
static int cv_twice(char *b1, char *b2, size_t n, const char *f, ...)
{
    va_list ap, ap2; int r1, r2;
    va_start(ap, f); va_copy(ap2, ap);
    r1 = tc_p_vsprintf_s(b1, n, f, ap); r2 = tc_p_vsprintf_s(b2, n, f, ap2);
    va_end(ap2); va_end(ap);
    return r1 == r2 ? r1 : -100;
}

TC_CASE(forward)
{
    char b[32]; volatile size_t n = 32; int r;
    r = cv(b, n, "%d:%s", 42, "xyz");
    CHECK_EQ_I(r, 6); CHECK_STR(b, "42:xyz");
}
TC_CASE(exact_fit)
{
    char b[8]; volatile size_t n = 7; int r;
    r = cv(b, n, "%s", "abcdef");
    CHECK_EQ_I(r, 6); CHECK_STR(b, "abcdef");
}
TC_CASE(empty_format)
{
    char b[8] = "QQQ"; volatile size_t n = 8; int r;
    r = cv(b, n, "");
    CHECK_EQ_I(r, 0); CHECK_EQ_I(b[0], 0);
}
TC_CASE(wide)
{
    wchar_t w[16]; volatile size_t n = 16; int r;
    r = cw(w, n, L"%d|%ls", 7, L"ab");
    CHECK_EQ_I(r, 4); CHECK_WSTR(w, L"7|ab");
}
TC_CASE(wide_exact_fit)
{
    wchar_t w[8]; volatile size_t n = 5; int r;
    r = cw(w, n, L"%ls", L"abcd");
    CHECK_EQ_I(r, 4); CHECK_WSTR(w, L"abcd");
}
TC_CASE(va_copy_twice)
{
    char a[32], b[32]; volatile size_t n = 32; int r;
    r = cv_twice(a, b, n, "%d-%s", 9, "k");
    CHECK_EQ_I(r, 3); CHECK_STR(a, "9-k"); CHECK_STR(b, "9-k");
}
int main(void)
{
    tc_begin();
#ifndef _UCRT
    TC_LOAD_PROC(vsprintf_s); TC_LOAD_PROC(vswprintf_s);
    if (!tc_p_vsprintf_s || !tc_p_vswprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__not exported\n"); return 2; }
#endif
    TC_RUN(forward); TC_RUN(exact_fit); TC_RUN(empty_format); TC_RUN(wide); TC_RUN(wide_exact_fit); TC_RUN(va_copy_twice);
    return tc_end();
}

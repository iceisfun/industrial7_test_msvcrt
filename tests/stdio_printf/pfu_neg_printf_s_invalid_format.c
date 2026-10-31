/* TC
 * id:    pfu_neg_printf_s_invalid_format
 * area:  stdio_printf
 * funcs: 
 * procs: printf_s wprintf_s vprintf_s vwprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs printf_s, wprintf_s, vprintf_s, vwprintf_s; printf format specification
 * note:  Writes to stdout, but the format is only the bad spec so nothing may be printed.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>
#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
/* secure printf wrappers: ucrtbase has no exported sprintf_s & co (they are header inlines over __stdio_common_*);
 * older CRTs export them, resolved at run time. P_<name> is the uniform call syntax. */
#if TC_CRT_VER >= 140
int __cdecl __stdio_common_vsprintf_s(unsigned __int64, char *, size_t, const char *, _locale_t, va_list);
int __cdecl __stdio_common_vswprintf_s(unsigned __int64, wchar_t *, size_t, const wchar_t *, _locale_t, va_list);
int __cdecl __stdio_common_vsnprintf_s(unsigned __int64, char *, size_t, size_t, const char *, _locale_t, va_list);
int __cdecl __stdio_common_vsnwprintf_s(unsigned __int64, wchar_t *, size_t, size_t, const wchar_t *, _locale_t, va_list);
int __cdecl __stdio_common_vfprintf_s(unsigned __int64, FILE *, const char *, _locale_t, va_list);
int __cdecl __stdio_common_vfwprintf_s(unsigned __int64, FILE *, const wchar_t *, _locale_t, va_list);
static int U_vprintf_s(const char *f, va_list ap) { return __stdio_common_vfprintf_s(0, stdout, f, NULL, ap); }
static int P_printf_s(const char *f, ...) { va_list ap; int r; va_start(ap, f); r = U_vprintf_s(f, ap); va_end(ap); return r; }
static int U_vwprintf_s(const wchar_t *f, va_list ap) { return __stdio_common_vfwprintf_s(0, stdout, f, NULL, ap); }
static int P_wprintf_s(const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = U_vwprintf_s(f, ap); va_end(ap); return r; }
static int P_vprintf_s(const char *f, va_list ap) { return U_vprintf_s(f, ap); }
static int P_vwprintf_s(const wchar_t *f, va_list ap) { return U_vwprintf_s(f, ap); }
#define LOAD_OR_DIE(n) ((void)0)
#else
TC_DECL_PROC(int, printf_s, (const char *, ...));
#define P_printf_s(...) tc_p_printf_s(__VA_ARGS__)
TC_DECL_PROC(int, wprintf_s, (const wchar_t *, ...));
#define P_wprintf_s(...) tc_p_wprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, vprintf_s, (const char *, va_list));
#define P_vprintf_s(...) tc_p_vprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, vwprintf_s, (const wchar_t *, va_list));
#define P_vwprintf_s(...) tc_p_vwprintf_s(__VA_ARGS__)
#define LOAD_OR_DIE(n) do { TC_LOAD_PROC(n); if (!tc_p_##n) { tc_write("TC FAIL " TC_TEST_ID "::__missing_" #n "\n"); return 1; } } while (0)
#endif
#define LOAD_ALL() do { LOAD_OR_DIE(printf_s); LOAD_OR_DIE(wprintf_s); LOAD_OR_DIE(vprintf_s); LOAD_OR_DIE(vwprintf_s); } while (0)

static int cv_printf_s(const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = P_vprintf_s(f, ap); va_end(ap); return r; }
static int cv_wprintf_s(const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = P_vwprintf_s(f, ap); va_end(ap); return r; }

TC_CASE(printf_s_percent_y)
{
    const char *volatile f = "%y"; int n; errno = 0; tc_iph_count = 0;
    n = P_printf_s(f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wprintf_s_percent_y)
{
    const wchar_t *volatile f = L"%y"; int n; errno = 0; tc_iph_count = 0;
    n = P_wprintf_s(f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(vprintf_s_percent_y)
{
    const char *volatile f = "%y"; int n; errno = 0; tc_iph_count = 0;
    n = cv_printf_s(f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(vwprintf_s_percent_y)
{
    const wchar_t *volatile f = L"%y"; int n; errno = 0; tc_iph_count = 0;
    n = cv_wprintf_s(f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); LOAD_ALL();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no_iph_api\n"); return 2; }
    TC_RUN(printf_s_percent_y); TC_RUN(wprintf_s_percent_y); TC_RUN(vprintf_s_percent_y); TC_RUN(vwprintf_s_percent_y);
    return tc_end();
}

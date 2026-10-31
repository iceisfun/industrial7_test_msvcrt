/* TC
 * id:    pfu_neg_snprintf_s_invalid_format
 * area:  stdio_printf
 * funcs: 
 * procs: _snprintf_s _snwprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf_s, _snwprintf_s; printf format specification (invalid conversion is an invalid parameter)
 * note:  Existing invalid_format tests cover sprintf_s/swprintf_s; this one covers the _snprintf_s family.
 *        snprintf_s/_snwprintf_s are called through P_ wrappers (ucrtbase: __stdio_common_vsnprintf_s; older CRTs: run-time lookup).
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
static int U_v_snprintf_s(char *b, size_t n, size_t c, const char *f, va_list ap) { return __stdio_common_vsnprintf_s(0, b, n, c, f, NULL, ap); }
static int P__snprintf_s(char *b, size_t n, size_t c, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = U_v_snprintf_s(b, n, c, f, ap); va_end(ap); return r; }
static int U_v_snwprintf_s(wchar_t *b, size_t n, size_t c, const wchar_t *f, va_list ap) { return __stdio_common_vsnwprintf_s(0, b, n, c, f, NULL, ap); }
static int P__snwprintf_s(wchar_t *b, size_t n, size_t c, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = U_v_snwprintf_s(b, n, c, f, ap); va_end(ap); return r; }
#define LOAD_OR_DIE(n) ((void)0)
#else
TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
#define P__snprintf_s(...) tc_p__snprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, _snwprintf_s, (wchar_t *, size_t, size_t, const wchar_t *, ...));
#define P__snwprintf_s(...) tc_p__snwprintf_s(__VA_ARGS__)
#define LOAD_OR_DIE(n) do { TC_LOAD_PROC(n); if (!tc_p_##n) { tc_write("TC FAIL " TC_TEST_ID "::__missing_" #n "\n"); return 1; } } while (0)
#endif
#define LOAD_ALL() do { LOAD_OR_DIE(_snprintf_s); LOAD_OR_DIE(_snwprintf_s); } while (0)
static void fillc(char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = 'Z'; }
static void fillw(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = L'Z'; }

TC_CASE(narrow_percent_y)
{
    char b[16]; const char *volatile f = "%y"; int n;
    fillc(b, 16); errno = 0; tc_iph_count = 0;
    n = P__snprintf_s(b, sizeof b, _TRUNCATE, f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(narrow_percent_y_count_lt_size)
{
    char b[16]; const char *volatile f = "%y"; int n;
    fillc(b, 16); errno = 0; tc_iph_count = 0;
    n = P__snprintf_s(b, sizeof b, 3, f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_percent_y)
{
    wchar_t b[16]; const wchar_t *volatile f = L"%y"; int n;
    fillw(b, 16); errno = 0; tc_iph_count = 0;
    n = P__snwprintf_s(b, 16, _TRUNCATE, f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(valid_after_failure)
{
    char b[16]; const char *volatile bad = "%y"; volatile int v = 12345; int n;
    tc_iph_count = 0;
    n = P__snprintf_s(b, sizeof b, _TRUNCATE, bad);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1);
    n = P__snprintf_s(b, sizeof b, _TRUNCATE, "%d", v);
    CHECK_EQ_I(n, 5); CHECK_STR(b, "12345"); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); LOAD_ALL();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no_iph_api\n"); return 2; }
    TC_RUN(narrow_percent_y); TC_RUN(narrow_percent_y_count_lt_size); TC_RUN(wide_percent_y); TC_RUN(valid_after_failure);
    return tc_end();
}

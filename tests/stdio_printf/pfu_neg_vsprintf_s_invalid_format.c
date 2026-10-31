/* TC
 * id:    pfu_neg_vsprintf_s_invalid_format
 * area:  stdio_printf
 * funcs: 
 * procs: vsprintf_s vswprintf_s _vsnprintf_s _vsnwprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs vsprintf_s, vswprintf_s, _vsnprintf_s, _vsnwprintf_s; printf format specification
 * note:  Invalid conversion character reached through the va_list entry points. Existing vsprintf_s/vsnprintf_s tests only cover NULL / too-small.
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
static int U_vsprintf_s(char *b, size_t n, const char *f, va_list ap) { return __stdio_common_vsprintf_s(0, b, n, f, NULL, ap); }
static int P_vsprintf_s(char *b, size_t n, const char *f, va_list ap) { return U_vsprintf_s(b, n, f, ap); }
static int U_vswprintf_s(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return __stdio_common_vswprintf_s(0, b, n, f, NULL, ap); }
static int P_vswprintf_s(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return U_vswprintf_s(b, n, f, ap); }
static int U_v_snprintf_s(char *b, size_t n, size_t c, const char *f, va_list ap) { return __stdio_common_vsnprintf_s(0, b, n, c, f, NULL, ap); }
static int P__vsnprintf_s(char *b, size_t n, size_t c, const char *f, va_list ap) { return U_v_snprintf_s(b, n, c, f, ap); }
static int U_v_snwprintf_s(wchar_t *b, size_t n, size_t c, const wchar_t *f, va_list ap) { return __stdio_common_vsnwprintf_s(0, b, n, c, f, NULL, ap); }
static int P__vsnwprintf_s(wchar_t *b, size_t n, size_t c, const wchar_t *f, va_list ap) { return U_v_snwprintf_s(b, n, c, f, ap); }
#define LOAD_OR_DIE(n) ((void)0)
#else
TC_DECL_PROC(int, vsprintf_s, (char *, size_t, const char *, va_list));
#define P_vsprintf_s(...) tc_p_vsprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, vswprintf_s, (wchar_t *, size_t, const wchar_t *, va_list));
#define P_vswprintf_s(...) tc_p_vswprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, _vsnprintf_s, (char *, size_t, size_t, const char *, va_list));
#define P__vsnprintf_s(...) tc_p__vsnprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, _vsnwprintf_s, (wchar_t *, size_t, size_t, const wchar_t *, va_list));
#define P__vsnwprintf_s(...) tc_p__vsnwprintf_s(__VA_ARGS__)
#define LOAD_OR_DIE(n) do { TC_LOAD_PROC(n); if (!tc_p_##n) { tc_write("TC FAIL " TC_TEST_ID "::__missing_" #n "\n"); return 1; } } while (0)
#endif
#define LOAD_ALL() do { LOAD_OR_DIE(vsprintf_s); LOAD_OR_DIE(vswprintf_s); LOAD_OR_DIE(_vsnprintf_s); LOAD_OR_DIE(_vsnwprintf_s); } while (0)
static void fillc(char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = 'Z'; }
static void fillw(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = L'Z'; }

static int cv_sprintf_s(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = P_vsprintf_s(b, n, f, ap); va_end(ap); return r; }
static int cv_swprintf_s(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = P_vswprintf_s(b, n, f, ap); va_end(ap); return r; }
static int cv_snprintf_s(char *b, size_t n, size_t c, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = P__vsnprintf_s(b, n, c, f, ap); va_end(ap); return r; }
static int cv_snwprintf_s(wchar_t *b, size_t n, size_t c, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = P__vsnwprintf_s(b, n, c, f, ap); va_end(ap); return r; }

TC_CASE(vsprintf_s_percent_y)
{
    char b[16]; const char *volatile f = "%y"; int n;
    fillc(b, 16); errno = 0; tc_iph_count = 0;
    n = cv_sprintf_s(b, sizeof b, f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(vswprintf_s_percent_y)
{
    wchar_t b[16]; const wchar_t *volatile f = L"%y"; int n;
    fillw(b, 16); errno = 0; tc_iph_count = 0;
    n = cv_swprintf_s(b, 16, f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(vsnprintf_s_percent_y_truncate)
{
    char b[16]; const char *volatile f = "%y"; int n;
    fillc(b, 16); errno = 0; tc_iph_count = 0;
    n = cv_snprintf_s(b, sizeof b, _TRUNCATE, f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(vsnwprintf_s_percent_y_truncate)
{
    wchar_t b[16]; const wchar_t *volatile f = L"%y"; int n;
    fillw(b, 16); errno = 0; tc_iph_count = 0;
    n = cv_snwprintf_s(b, 16, _TRUNCATE, f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); LOAD_ALL();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no_iph_api\n"); return 2; }
    TC_RUN(vsprintf_s_percent_y); TC_RUN(vswprintf_s_percent_y); TC_RUN(vsnprintf_s_percent_y_truncate); TC_RUN(vsnwprintf_s_percent_y_truncate);
    return tc_end();
}

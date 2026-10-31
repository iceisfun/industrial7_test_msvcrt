/* TC
 * id:    pfu_neg_probe_snprintf_s_bad_format_buffer
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf_s __stdio_common_vsnprintf_s __stdio_common_vsnwprintf_s
 * procs: 
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs sprintf_s: on failure buffer[0] is set to 0 (documented for too-small buffers; extended here to invalid conversion)
 * note:  PROBES: buffer state after an invalid conversion char in text that already produced output ("abc%y"): buffer[0]==0.
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
static int P_sprintf_s(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = U_vsprintf_s(b, n, f, ap); va_end(ap); return r; }
static int U_v_snprintf_s(char *b, size_t n, size_t c, const char *f, va_list ap) { return __stdio_common_vsnprintf_s(0, b, n, c, f, NULL, ap); }
static int P__snprintf_s(char *b, size_t n, size_t c, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = U_v_snprintf_s(b, n, c, f, ap); va_end(ap); return r; }
static int U_v_snwprintf_s(wchar_t *b, size_t n, size_t c, const wchar_t *f, va_list ap) { return __stdio_common_vsnwprintf_s(0, b, n, c, f, NULL, ap); }
static int P__snwprintf_s(wchar_t *b, size_t n, size_t c, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = U_v_snwprintf_s(b, n, c, f, ap); va_end(ap); return r; }
static int P_vsprintf_s(char *b, size_t n, const char *f, va_list ap) { return U_vsprintf_s(b, n, f, ap); }
#define LOAD_OR_DIE(n) ((void)0)
#else
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#define P_sprintf_s(...) tc_p_sprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
#define P__snprintf_s(...) tc_p__snprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, _snwprintf_s, (wchar_t *, size_t, size_t, const wchar_t *, ...));
#define P__snwprintf_s(...) tc_p__snwprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, vsprintf_s, (char *, size_t, const char *, va_list));
#define P_vsprintf_s(...) tc_p_vsprintf_s(__VA_ARGS__)
#define LOAD_OR_DIE(n) do { TC_LOAD_PROC(n); if (!tc_p_##n) { tc_write("TC FAIL " TC_TEST_ID "::__missing_" #n "\n"); return 1; } } while (0)
#endif
#define LOAD_ALL() do { LOAD_OR_DIE(sprintf_s); LOAD_OR_DIE(_snprintf_s); LOAD_OR_DIE(_snwprintf_s); LOAD_OR_DIE(vsprintf_s); } while (0)
static void fillc(char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = 'Z'; }
static void fillw(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = L'Z'; }

static int cv_sprintf_s(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = P_vsprintf_s(b, n, f, ap); va_end(ap); return r; }
TC_CASE(snprintf_s_truncate_abc_y)
{ char b[16]; const char *volatile f = "abc%y"; fillc(b, 16); P__snprintf_s(b, sizeof b, _TRUNCATE, f); CHECK_EQ_I(b[0], 0); }
TC_CASE(snwprintf_s_abc_y)
{ wchar_t b[16]; const wchar_t *volatile f = L"abc%y"; fillw(b, 16); P__snwprintf_s(b, 16, _TRUNCATE, f); CHECK_EQ_I(b[0], 0); }
TC_CASE(vsprintf_s_abc_y)
{ char b[16]; const char *volatile f = "abc%y"; fillc(b, 16); cv_sprintf_s(b, sizeof b, f); CHECK_EQ_I(b[0], 0); }
TC_CASE(sprintf_s_size_one_y)
{ char b[16]; const char *volatile f = "%y"; fillc(b, 16); P_sprintf_s(b, 1, f); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[1], 'Z'); }
int main(void)
{
    tc_begin(); LOAD_ALL();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no_iph_api\n"); return 2; }
    TC_RUN(snprintf_s_truncate_abc_y); TC_RUN(snwprintf_s_abc_y); TC_RUN(vsprintf_s_abc_y); TC_RUN(sprintf_s_size_one_y);
    return tc_end();
}

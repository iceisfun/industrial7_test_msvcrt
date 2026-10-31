/* TC
 * id:    pfu_neg_handler_args_release_probe
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf_s __stdio_common_vsnprintf_s __stdio_common_vfprintf_s
 * procs: 
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   release UCRT calls the invalid parameter handler through _invalid_parameter_noinfo: expression, function, file NULL, line 0, reserved 0
 * note:  PROBES: arguments passed to the handler by the release ucrtbase for secure printf failures. Only the argument values are probed.
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
static int U_vfprintf_s(FILE *s, const char *f, va_list ap) { return __stdio_common_vfprintf_s(0, s, f, NULL, ap); }
static int P_fprintf_s(FILE *s, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = U_vfprintf_s(s, f, ap); va_end(ap); return r; }
#define LOAD_OR_DIE(n) ((void)0)
#else
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#define P_sprintf_s(...) tc_p_sprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
#define P__snprintf_s(...) tc_p__snprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, fprintf_s, (FILE *, const char *, ...));
#define P_fprintf_s(...) tc_p_fprintf_s(__VA_ARGS__)
#define LOAD_OR_DIE(n) do { TC_LOAD_PROC(n); if (!tc_p_##n) { tc_write("TC FAIL " TC_TEST_ID "::__missing_" #n "\n"); return 1; } } while (0)
#endif
#define LOAD_ALL() do { LOAD_OR_DIE(sprintf_s); LOAD_OR_DIE(_snprintf_s); LOAD_OR_DIE(fprintf_s); } while (0)

static const wchar_t *g_e, *g_f, *g_file; static unsigned int g_line; static uintptr_t g_res; static int g_n;
static void __cdecl rec(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ g_n++; g_e = e; g_f = f; g_file = fl; g_line = l; g_res = r; }
static void reset(void) { g_n = 0; g_e = g_f = g_file = (const wchar_t *)(uintptr_t)1; g_line = 99; g_res = 99; }
static void check_args(void)
{
    CHECK_EQ_I(g_n, 1);
    CHECK_EQ_P(g_e, NULL); CHECK_EQ_P(g_f, NULL); CHECK_EQ_P(g_file, NULL);
    CHECK_EQ_U(g_line, 0); CHECK_EQ_U(g_res, 0);
}
TC_CASE(sprintf_s_overflow)
{
    char b[2]; volatile int v = 12345; int r; reset();
    r = P_sprintf_s(b, sizeof b, "%d", v);
    CHECK_EQ_I(r, -1); check_args();
}
TC_CASE(fprintf_s_null_stream)
{
    FILE *volatile nf = NULL; int r; reset();
    r = P_fprintf_s(nf, "x");
    CHECK_EQ_I(r, -1); check_args();
}
TC_CASE(snprintf_s_null_buffer)
{
    char *volatile nb = NULL; int r; reset();
    r = P__snprintf_s(nb, 16, _TRUNCATE, "x");
    CHECK_EQ_I(r, -1); check_args();
}
int main(void)
{
    tc_iph_t (__cdecl *set)(tc_iph_t);
    tc_begin();
    set = (tc_iph_t (__cdecl *)(tc_iph_t))tc_crt_proc("_set_invalid_parameter_handler");
    if (!set) { tc_write("TC FAIL " TC_TEST_ID "::__no_iph_api\n"); return 2; }
    set(rec);
    TC_RUN(sprintf_s_overflow); TC_RUN(fprintf_s_null_stream); TC_RUN(snprintf_s_null_buffer);
    return tc_end();
}

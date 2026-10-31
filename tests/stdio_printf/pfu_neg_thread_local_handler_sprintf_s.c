/* TC
 * id:    pfu_neg_thread_local_handler_sprintf_s
 * area:  stdio_printf
 * funcs: _set_thread_local_invalid_parameter_handler
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_thread_local_invalid_parameter_handler: the thread-local handler is used instead of the global one for the calling thread
 * note:  ucrtbase only: _set_thread_local_invalid_parameter_handler is not exported by msvcr120 and older.
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

#include <stdlib.h>
static int local_cnt;
static void __cdecl local_h(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; local_cnt++; }

TC_CASE(local_gets_call)
{
    char b[4]; volatile int v = 123456; int r;
    _set_thread_local_invalid_parameter_handler(local_h);
    local_cnt = 0; tc_iph_count = 0; errno = 0;
    r = P_sprintf_s(b, sizeof b, "%d", v);
    _set_thread_local_invalid_parameter_handler(NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(local_cnt, 1); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(null_stream_fprintf_s)
{
    FILE *volatile nf = NULL; int r;
    _set_thread_local_invalid_parameter_handler(local_h);
    local_cnt = 0; tc_iph_count = 0; errno = 0;
    r = P_fprintf_s(nf, "x");
    _set_thread_local_invalid_parameter_handler(NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(local_cnt, 1); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(reset_local_null_falls_back_to_global)
{
    char b[4]; volatile int v = 123456; int r;
    _set_thread_local_invalid_parameter_handler(local_h);
    _set_thread_local_invalid_parameter_handler(NULL);
    local_cnt = 0; tc_iph_count = 0; errno = 0;
    r = P_sprintf_s(b, sizeof b, "%d", v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(local_cnt, 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(local_handler_return_continues)
{
    char b[4]; volatile int v = 123456; int r1, r2;
    _set_thread_local_invalid_parameter_handler(local_h);
    local_cnt = 0; tc_iph_count = 0;
    r1 = P__snprintf_s(b, sizeof b, 10, "%d", v);
    r2 = P__snprintf_s(b, sizeof b, _TRUNCATE, "%d", 12);
    _set_thread_local_invalid_parameter_handler(NULL);
    CHECK_EQ_I(r1, -1); CHECK_EQ_I(local_cnt, 1); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(r2, 2); CHECK_STR(b, "12");
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no_iph_api\n"); return 2; }
    TC_RUN(local_gets_call); TC_RUN(null_stream_fprintf_s); TC_RUN(reset_local_null_falls_back_to_global); TC_RUN(local_handler_return_continues);
    return tc_end();
}

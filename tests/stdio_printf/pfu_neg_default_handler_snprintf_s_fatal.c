/* TC
 * id:    pfu_neg_default_handler_snprintf_s_fatal
 * area:  stdio_printf
 * funcs: 
 * procs: _snprintf_s _vsnprintf_s _snwprintf_s vsprintf_s vswprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter Validation; _snprintf_s, _vsnprintf_s, _snwprintf_s, vsprintf_s, vswprintf_s: overflow/NULL/bad format invoke the invalid parameter handler (default terminates); _TRUNCATE does not
 * note:  Each fatal call runs in a child process with NO handler installed.
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
static int P__vsnprintf_s(char *b, size_t n, size_t c, const char *f, va_list ap) { return U_v_snprintf_s(b, n, c, f, ap); }
static int U_v_snwprintf_s(wchar_t *b, size_t n, size_t c, const wchar_t *f, va_list ap) { return __stdio_common_vsnwprintf_s(0, b, n, c, f, NULL, ap); }
static int P__snwprintf_s(wchar_t *b, size_t n, size_t c, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = U_v_snwprintf_s(b, n, c, f, ap); va_end(ap); return r; }
static int U_vsprintf_s(char *b, size_t n, const char *f, va_list ap) { return __stdio_common_vsprintf_s(0, b, n, f, NULL, ap); }
static int P_vsprintf_s(char *b, size_t n, const char *f, va_list ap) { return U_vsprintf_s(b, n, f, ap); }
static int U_vswprintf_s(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return __stdio_common_vswprintf_s(0, b, n, f, NULL, ap); }
static int P_vswprintf_s(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return U_vswprintf_s(b, n, f, ap); }
#define LOAD_OR_DIE(n) ((void)0)
#else
TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
#define P__snprintf_s(...) tc_p__snprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, _vsnprintf_s, (char *, size_t, size_t, const char *, va_list));
#define P__vsnprintf_s(...) tc_p__vsnprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, _snwprintf_s, (wchar_t *, size_t, size_t, const wchar_t *, ...));
#define P__snwprintf_s(...) tc_p__snwprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, vsprintf_s, (char *, size_t, const char *, va_list));
#define P_vsprintf_s(...) tc_p_vsprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, vswprintf_s, (wchar_t *, size_t, const wchar_t *, va_list));
#define P_vswprintf_s(...) tc_p_vswprintf_s(__VA_ARGS__)
#define LOAD_OR_DIE(n) do { TC_LOAD_PROC(n); if (!tc_p_##n) { tc_write("TC FAIL " TC_TEST_ID "::__missing_" #n "\n"); return 1; } } while (0)
#endif
#define LOAD_ALL() do { LOAD_OR_DIE(_snprintf_s); LOAD_OR_DIE(_vsnprintf_s); LOAD_OR_DIE(_snwprintf_s); LOAD_OR_DIE(vsprintf_s); LOAD_OR_DIE(vswprintf_s); } while (0)

static int cv_snprintf_s(char *b, size_t n, size_t c, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = P__vsnprintf_s(b, n, c, f, ap); va_end(ap); return r; }
static int cv_sprintf_s(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = P_vsprintf_s(b, n, f, ap); va_end(ap); return r; }
static int cv_swprintf_s(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = P_vswprintf_s(b, n, f, ap); va_end(ap); return r; }

static int child_main(const char *c)
{
    char b[4]; wchar_t w[8];
    char *volatile nb = NULL; const wchar_t *volatile nwfmt = NULL;
    const wchar_t *volatile badw = L"%y"; volatile int v = 123456;
    tc_iph_default();
    if (tc_strequ(c, "snprintf_s_overflow_count_ge_size")) { P__snprintf_s(b, sizeof b, 10, "%d", v); return 77; }
    if (tc_strequ(c, "vsnprintf_s_null_buffer")) { cv_snprintf_s(nb, 16, 10, "x"); return 77; }
    if (tc_strequ(c, "snwprintf_s_bad_format")) { P__snwprintf_s(w, 8, _TRUNCATE, badw); return 77; }
    if (tc_strequ(c, "vsprintf_s_overflow")) { cv_sprintf_s(b, 2, "%d", v); return 77; }
    if (tc_strequ(c, "vswprintf_s_null_format")) { cv_swprintf_s(w, 8, nwfmt); return 77; }
    if (tc_strequ(c, "control_truncate_not_fatal")) {
        int n = P__snprintf_s(b, sizeof b, _TRUNCATE, "%s", "abcdef");
        return (n == -1 && b[0] == 'a' && b[1] == 'b' && b[2] == 'c' && b[3] == 0) ? 77 : 99;
    }
    return 97;
}
static void expect_fatal(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK(code != 0);
}
TC_CASE(snprintf_s_overflow_count_ge_size) { expect_fatal("snprintf_s_overflow_count_ge_size"); }
TC_CASE(vsnprintf_s_null_buffer) { expect_fatal("vsnprintf_s_null_buffer"); }
TC_CASE(snwprintf_s_bad_format) { expect_fatal("snwprintf_s_bad_format"); }
TC_CASE(vsprintf_s_overflow) { expect_fatal("vsprintf_s_overflow"); }
TC_CASE(vswprintf_s_null_format) { expect_fatal("vswprintf_s_null_format"); }
TC_CASE(control_truncate_not_fatal)
{
    DWORD code = 0; int ran = tc_spawn_self("control_truncate_not_fatal", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
}
int main(void)
{
    const char *c;
    tc_begin(); LOAD_ALL();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(snprintf_s_overflow_count_ge_size); TC_RUN(vsnprintf_s_null_buffer); TC_RUN(snwprintf_s_bad_format);
    TC_RUN(vsprintf_s_overflow); TC_RUN(vswprintf_s_null_format); TC_RUN(control_truncate_not_fatal);
    return tc_end();
}

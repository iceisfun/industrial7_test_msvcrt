/* TC
 * id:    pfu_neg_default_handler_stream_fatal
 * area:  stdio_printf
 * funcs: fopen fclose
 * procs: fprintf_s fwprintf_s vfprintf_s printf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter Validation / _set_invalid_parameter_handler: without a handler an invalid parameter terminates the process; fprintf_s, fwprintf_s, vfprintf_s, printf_s
 * note:  Each fatal call runs in a child process with NO handler installed. Only 'process did not return 77 and exit code != 0' is asserted (exact status is probed elsewhere).
 *        HARNESS FIX (class F, Win10 22H2 x64 (19045) ucrtbase.dll observed, zzp_probe5/6: every printf/scanf/strcpy_s "default handler" child exited 0x4d = survived): the mingw-w64 startup code installs
 *        a no-op invalid-parameter handler (call to _set_invalid_parameter_handler(__mingw_invalidParameterHandler) in main's crt startup), so the CRT's own
 *        default (terminate) never ran. The child now calls tc_iph_default() (= _set_invalid_parameter_handler(NULL)) first. Test expectations unchanged.
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
static int U_vfprintf_s(FILE *s, const char *f, va_list ap) { return __stdio_common_vfprintf_s(0, s, f, NULL, ap); }
static int P_fprintf_s(FILE *s, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = U_vfprintf_s(s, f, ap); va_end(ap); return r; }
static int U_vfwprintf_s(FILE *s, const wchar_t *f, va_list ap) { return __stdio_common_vfwprintf_s(0, s, f, NULL, ap); }
static int P_fwprintf_s(FILE *s, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = U_vfwprintf_s(s, f, ap); va_end(ap); return r; }
static int P_vfprintf_s(FILE *s, const char *f, va_list ap) { return U_vfprintf_s(s, f, ap); }
static int U_vprintf_s(const char *f, va_list ap) { return __stdio_common_vfprintf_s(0, stdout, f, NULL, ap); }
static int P_printf_s(const char *f, ...) { va_list ap; int r; va_start(ap, f); r = U_vprintf_s(f, ap); va_end(ap); return r; }
#define LOAD_OR_DIE(n) ((void)0)
#else
TC_DECL_PROC(int, fprintf_s, (FILE *, const char *, ...));
#define P_fprintf_s(...) tc_p_fprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, fwprintf_s, (FILE *, const wchar_t *, ...));
#define P_fwprintf_s(...) tc_p_fwprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, vfprintf_s, (FILE *, const char *, va_list));
#define P_vfprintf_s(...) tc_p_vfprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, printf_s, (const char *, ...));
#define P_printf_s(...) tc_p_printf_s(__VA_ARGS__)
#define LOAD_OR_DIE(n) do { TC_LOAD_PROC(n); if (!tc_p_##n) { tc_write("TC FAIL " TC_TEST_ID "::__missing_" #n "\n"); return 1; } } while (0)
#endif
#define LOAD_ALL() do { LOAD_OR_DIE(fprintf_s); LOAD_OR_DIE(fwprintf_s); LOAD_OR_DIE(vfprintf_s); LOAD_OR_DIE(printf_s); } while (0)

#define FN "tc_pfu_neg_default_handler_stream_fatal_" TC_CRT_DLL ".tmp"

static int cv_fprintf_s(FILE *s, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = P_vfprintf_s(s, f, ap); va_end(ap); return r; }

static int child_main(const char *c)
{
    FILE *volatile nf = NULL; const char *volatile nfmt = NULL;
    tc_iph_default(); const wchar_t *volatile nwfmt = NULL;
    const char *volatile bad = "%y";
    if (tc_strequ(c, "fprintf_s_null_stream")) { P_fprintf_s(nf, "x"); return 77; }
    if (tc_strequ(c, "fwprintf_s_null_format")) { P_fwprintf_s(stdout, nwfmt); return 77; }
    if (tc_strequ(c, "vfprintf_s_null_stream")) { cv_fprintf_s(nf, "x"); return 77; }
    if (tc_strequ(c, "fprintf_s_percent_y")) { P_fprintf_s(stdout, bad); return 77; }
    if (tc_strequ(c, "printf_s_null_format")) { P_printf_s(nfmt); return 77; }
    if (tc_strequ(c, "control_valid")) {
        FILE *f = fopen(FN, "w"); int n;
        if (!f) return 98;
        n = P_fprintf_s(f, "abc"); fclose(f); remove(FN);
        return n == 3 ? 77 : 99;
    }
    return 97;
}
static void expect_fatal(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK(code != 0);
}
TC_CASE(fprintf_s_null_stream) { expect_fatal("fprintf_s_null_stream"); }
TC_CASE(fwprintf_s_null_format) { expect_fatal("fwprintf_s_null_format"); }
TC_CASE(vfprintf_s_null_stream) { expect_fatal("vfprintf_s_null_stream"); }
TC_CASE(fprintf_s_percent_y) { expect_fatal("fprintf_s_percent_y"); }
TC_CASE(printf_s_null_format) { expect_fatal("printf_s_null_format"); }
TC_CASE(control_valid_not_fatal)
{
    DWORD code = 0; int ran = tc_spawn_self("control_valid", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
}
int main(void)
{
    const char *c;
    tc_begin(); LOAD_ALL();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    remove(FN);
    TC_RUN(fprintf_s_null_stream); TC_RUN(fwprintf_s_null_format); TC_RUN(vfprintf_s_null_stream);
    TC_RUN(fprintf_s_percent_y); TC_RUN(printf_s_null_format); TC_RUN(control_valid_not_fatal);
    return tc_end();
}

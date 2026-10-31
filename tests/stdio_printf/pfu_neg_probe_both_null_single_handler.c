/* TC
 * id:    pfu_neg_probe_both_null_single_handler
 * area:  stdio_printf
 * funcs: 
 * procs: sprintf_s swprintf_s _snprintf_s vsprintf_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs sprintf_s family: NULL buffer or NULL format is an invalid parameter (EINVAL)
 * note:  PROBES: when BOTH buffer and format are NULL, exactly ONE handler call occurs (validation stops at the first failed check). Each NULL on its own is documented and tested elsewhere.
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
static int U_vswprintf_s(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return __stdio_common_vswprintf_s(0, b, n, f, NULL, ap); }
static int P_swprintf_s(wchar_t *b, size_t n, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = U_vswprintf_s(b, n, f, ap); va_end(ap); return r; }
static int U_v_snprintf_s(char *b, size_t n, size_t c, const char *f, va_list ap) { return __stdio_common_vsnprintf_s(0, b, n, c, f, NULL, ap); }
static int P__snprintf_s(char *b, size_t n, size_t c, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = U_v_snprintf_s(b, n, c, f, ap); va_end(ap); return r; }
static int P_vsprintf_s(char *b, size_t n, const char *f, va_list ap) { return U_vsprintf_s(b, n, f, ap); }
#define LOAD_OR_DIE(n) ((void)0)
#else
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#define P_sprintf_s(...) tc_p_sprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, swprintf_s, (wchar_t *, size_t, const wchar_t *, ...));
#define P_swprintf_s(...) tc_p_swprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
#define P__snprintf_s(...) tc_p__snprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, vsprintf_s, (char *, size_t, const char *, va_list));
#define P_vsprintf_s(...) tc_p_vsprintf_s(__VA_ARGS__)
#define LOAD_OR_DIE(n) do { TC_LOAD_PROC(n); if (!tc_p_##n) { tc_write("TC FAIL " TC_TEST_ID "::__missing_" #n "\n"); return 1; } } while (0)
#endif
#define LOAD_ALL() do { LOAD_OR_DIE(sprintf_s); LOAD_OR_DIE(swprintf_s); LOAD_OR_DIE(_snprintf_s); LOAD_OR_DIE(vsprintf_s); } while (0)

static int cv_sprintf_s(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = P_vsprintf_s(b, n, f, ap); va_end(ap); return r; }
static char *volatile nb; static wchar_t *volatile nwb; static const char *volatile nf; static const wchar_t *volatile nwf;

TC_CASE(sprintf_s_both_null)  { int r; errno = 0; tc_iph_count = 0; r = P_sprintf_s(nb, 10, nf); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(swprintf_s_both_null) { int r; errno = 0; tc_iph_count = 0; r = P_swprintf_s(nwb, 10, nwf); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(snprintf_s_both_null) { int r; errno = 0; tc_iph_count = 0; r = P__snprintf_s(nb, 10, _TRUNCATE, nf); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(vsprintf_s_both_null) { int r; errno = 0; tc_iph_count = 0; r = cv_sprintf_s(nb, 10, nf); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin(); LOAD_ALL();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no_iph_api\n"); return 2; }
    TC_RUN(sprintf_s_both_null); TC_RUN(swprintf_s_both_null); TC_RUN(snprintf_s_both_null); TC_RUN(vsprintf_s_both_null);
    return tc_end();
}

/* TC
 * id:    pfu_neg_probe_fprintf_s_invalid_format_partial
 * area:  stdio_printf
 * funcs: fopen fclose fread
 * procs: 
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   UCRT output engine writes literal text to the stream as it is scanned; the invalid conversion aborts afterwards
 * note:  PROBES: output already written before an invalid conversion char ("abc%y") - whether the prefix "abc" reaches the file. Return/errno/handler behaviour is documented and tested in pfu_neg_fprintf_s_invalid_format.
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
#define LOAD_OR_DIE(n) ((void)0)
#else
TC_DECL_PROC(int, fprintf_s, (FILE *, const char *, ...));
#define P_fprintf_s(...) tc_p_fprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, fwprintf_s, (FILE *, const wchar_t *, ...));
#define P_fwprintf_s(...) tc_p_fwprintf_s(__VA_ARGS__)
#define LOAD_OR_DIE(n) do { TC_LOAD_PROC(n); if (!tc_p_##n) { tc_write("TC FAIL " TC_TEST_ID "::__missing_" #n "\n"); return 1; } } while (0)
#endif
#define LOAD_ALL() do { LOAD_OR_DIE(fprintf_s); LOAD_OR_DIE(fwprintf_s); } while (0)

#define FN "tc_pfu_neg_probe_fprintf_s_invalid_format_partial_" TC_CRT_DLL ".tmp"
static char content[32];
static int slurp(void)
{
    FILE *f = fopen(FN, "rb"); size_t n = 0; int i;
    for (i = 0; i < 32; i++) content[i] = 0;
    if (!f) return -1;
    n = fread(content, 1, 31, f); fclose(f); return (int)n;
}
TC_CASE(fprintf_s_abc_y)
{
    FILE *f = fopen(FN, "w"); const char *volatile fm = "abc%y"; int n;
    CHECK(f != NULL); if (!f) return;
    n = P_fprintf_s(f, fm); fclose(f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(slurp(), 3); CHECK_STR(content, "abc"); remove(FN);
}
TC_CASE(fwprintf_s_abc_y)
{
    FILE *f = fopen(FN, "w"); const wchar_t *volatile fm = L"abc%y"; int n;
    CHECK(f != NULL); if (!f) return;
    n = P_fwprintf_s(f, fm); fclose(f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(slurp(), 3); CHECK_STR(content, "abc"); remove(FN);
}
int main(void)
{
    tc_begin(); remove(FN);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no_iph_api\n"); return 2; }
    TC_RUN(fprintf_s_abc_y); TC_RUN(fwprintf_s_abc_y);
    return tc_end();
}

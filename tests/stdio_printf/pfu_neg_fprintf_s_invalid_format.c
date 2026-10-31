/* TC
 * id:    pfu_neg_fprintf_s_invalid_format
 * area:  stdio_printf
 * funcs: fopen fclose
 * procs: fprintf_s fwprintf_s vfprintf_s vfwprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fprintf_s, fwprintf_s, vfprintf_s, vfwprintf_s; printf format specification
 * note:  Format is only the bad spec so nothing may reach the file. Temp file tc_pfu_neg_fprintf_s_invalid_format_<dll>.tmp.
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
static int P_vfwprintf_s(FILE *s, const wchar_t *f, va_list ap) { return U_vfwprintf_s(s, f, ap); }
#define LOAD_OR_DIE(n) ((void)0)
#else
TC_DECL_PROC(int, fprintf_s, (FILE *, const char *, ...));
#define P_fprintf_s(...) tc_p_fprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, fwprintf_s, (FILE *, const wchar_t *, ...));
#define P_fwprintf_s(...) tc_p_fwprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, vfprintf_s, (FILE *, const char *, va_list));
#define P_vfprintf_s(...) tc_p_vfprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, vfwprintf_s, (FILE *, const wchar_t *, va_list));
#define P_vfwprintf_s(...) tc_p_vfwprintf_s(__VA_ARGS__)
#define LOAD_OR_DIE(n) do { TC_LOAD_PROC(n); if (!tc_p_##n) { tc_write("TC FAIL " TC_TEST_ID "::__missing_" #n "\n"); return 1; } } while (0)
#endif
#define LOAD_ALL() do { LOAD_OR_DIE(fprintf_s); LOAD_OR_DIE(fwprintf_s); LOAD_OR_DIE(vfprintf_s); LOAD_OR_DIE(vfwprintf_s); } while (0)

#define FN "tc_pfu_neg_fprintf_s_invalid_format_" TC_CRT_DLL ".tmp"

static int cv_fprintf_s(FILE *s, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = P_vfprintf_s(s, f, ap); va_end(ap); return r; }
static int cv_fwprintf_s(FILE *s, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = P_vfwprintf_s(s, f, ap); va_end(ap); return r; }
static long flen(void)
{
    FILE *f = fopen(FN, "rb"); long n = -1;
    if (f) { if (fseek(f, 0, SEEK_END) == 0) n = ftell(f); fclose(f); }
    return n;
}

TC_CASE(fprintf_s_percent_y)
{
    FILE *f = fopen(FN, "w"); const char *volatile fm = "%y"; int n;
    CHECK(f != NULL); if (!f) return;
    errno = 0; tc_iph_count = 0;
    n = P_fprintf_s(f, fm);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); CHECK_EQ_I(flen(), 0); remove(FN);
}
TC_CASE(fwprintf_s_percent_y)
{
    FILE *f = fopen(FN, "w"); const wchar_t *volatile fm = L"%y"; int n;
    CHECK(f != NULL); if (!f) return;
    errno = 0; tc_iph_count = 0;
    n = P_fwprintf_s(f, fm);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); CHECK_EQ_I(flen(), 0); remove(FN);
}
TC_CASE(vfprintf_s_percent_y)
{
    FILE *f = fopen(FN, "w"); const char *volatile fm = "%y"; int n;
    CHECK(f != NULL); if (!f) return;
    errno = 0; tc_iph_count = 0;
    n = cv_fprintf_s(f, fm);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); CHECK_EQ_I(flen(), 0); remove(FN);
}
TC_CASE(vfwprintf_s_percent_y)
{
    FILE *f = fopen(FN, "w"); const wchar_t *volatile fm = L"%y"; int n;
    CHECK(f != NULL); if (!f) return;
    errno = 0; tc_iph_count = 0;
    n = cv_fwprintf_s(f, fm);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); CHECK_EQ_I(flen(), 0); remove(FN);
}
TC_CASE(stream_usable_after)
{
    FILE *f = fopen(FN, "w"); const char *volatile bad = "%y"; int n;
    CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0;
    n = P_fprintf_s(f, bad);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1);
    n = P_fprintf_s(f, "abc");
    CHECK_EQ_I(n, 3); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); CHECK_EQ_I(flen(), 3); remove(FN);
}
int main(void)
{
    tc_begin(); LOAD_ALL(); remove(FN);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no_iph_api\n"); return 2; }
    TC_RUN(fprintf_s_percent_y); TC_RUN(fwprintf_s_percent_y); TC_RUN(vfprintf_s_percent_y); TC_RUN(vfwprintf_s_percent_y); TC_RUN(stream_usable_after);
    return tc_end();
}

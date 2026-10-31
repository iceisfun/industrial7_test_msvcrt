/* TC
 * id:    ucr_stdio_vsprintf_std_snprintf
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsprintf
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs snprintf, vsnprintf (UCRT: C99 behaviour, _CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR = 2)
 * note:  option bit 0x2 is the value of _CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR in corecrt_stdio_config.h
 */
#include "tc.h"
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

typedef int (__cdecl *vsprintf_t)(unsigned __int64, char *, size_t, const char *, void *, va_list);
typedef int (__cdecl *vsprintf_s_t)(unsigned __int64, char *, size_t, const char *, void *, va_list);
typedef int (__cdecl *vsnprintf_s_t)(unsigned __int64, char *, size_t, size_t, const char *, void *, va_list);
typedef int (__cdecl *vswprintf_t)(unsigned __int64, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vsprintf_t p_vsprintf;
static vsprintf_s_t p_vsprintf_s;
static vsnprintf_s_t p_vsnprintf_s;
static vswprintf_t p_vswprintf, p_vswprintf_s;

static int W_sp(unsigned __int64 o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static int W_sps(unsigned __int64 o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsprintf_s(o, b, n, f, NULL, ap); va_end(ap); return r; }
static int W_snps(unsigned __int64 o, char *b, size_t n, size_t c, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsnprintf_s(o, b, n, c, f, NULL, ap); va_end(ap); return r; }
static int W_wsp(unsigned __int64 o, wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vswprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static int W_wsps(unsigned __int64 o, wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vswprintf_s(o, b, n, f, NULL, ap); va_end(ap); return r; }

static void load(void)
{
    p_vsprintf = (vsprintf_t)tc_crt_proc("__stdio_common_vsprintf");
    p_vsprintf_s = (vsprintf_s_t)tc_crt_proc("__stdio_common_vsprintf_s");
    p_vsnprintf_s = (vsnprintf_s_t)tc_crt_proc("__stdio_common_vsnprintf_s");
    p_vswprintf = (vswprintf_t)tc_crt_proc("__stdio_common_vswprintf");
    p_vswprintf_s = (vswprintf_t)tc_crt_proc("__stdio_common_vswprintf_s");
}
#define NEED(p) do { if (!(p)) TC_SKIP("not exported"); } while (0)
#define RESET() do { errno = 0; tc_iph_count = 0; } while (0)
#define STD 2ULL

TC_CASE(fits)
{
    char b[16]; int n;
    NEED(p_vsprintf);
    n = W_sp(STD, b, sizeof b, "abc%d", 12);
    CHECK_EQ_I(n, 5); CHECK_STR(b, "abc12");
}
TC_CASE(exact_fit)
{
    char b[8]; int n;
    NEED(p_vsprintf);
    b[5] = 'x';
    n = W_sp(STD, b, 6, "abcde");
    CHECK_EQ_I(n, 5); CHECK_STR(b, "abcde"); CHECK_EQ_I(b[5], 0);
}
TC_CASE(size_query)
{
    int n;
    NEED(p_vsprintf);
    RESET();
    n = W_sp(STD, NULL, 0, "%s=%d", "value", 12345);
    CHECK_EQ_I(n, 11);
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin(); load(); tc_iph_install();
    TC_RUN(fits); TC_RUN(exact_fit); TC_RUN(size_query);
    return tc_end();
}

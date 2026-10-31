/* TC
 * id:    ucr_neg_stdio_vsnprintf_s_truncate
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsnprintf_s
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf_s: _TRUNCATE truncates and returns -1 without invoking the handler; count limits characters stored
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

TC_CASE(truncate4)
{
    char b[8]; int n;
    NEED(p_vsnprintf_s);
    memset(b, 'S', sizeof b);
    RESET();
    n = W_snps(0, b, 4, (size_t)-1, "abcdefgh");
    CHECK_EQ_I(n, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(b[4], 'S');
}
TC_CASE(truncate1)
{
    char b[4]; int n;
    NEED(p_vsnprintf_s);
    memset(b, 'S', sizeof b);
    RESET();
    n = W_snps(0, b, 1, (size_t)-1, "abc");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_limits)
{
    char b[16]; int n;
    NEED(p_vsnprintf_s);
    memset(b, 'S', sizeof b);
    RESET();
    n = W_snps(0, b, 16, 3, "abcdefgh");
    CHECK_EQ_I(n, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin(); load(); tc_iph_install();
    TC_RUN(truncate4); TC_RUN(truncate1); TC_RUN(count_limits);
    return tc_end();
}

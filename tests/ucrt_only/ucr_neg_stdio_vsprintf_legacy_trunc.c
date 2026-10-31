/* TC
 * id:    ucr_neg_stdio_vsprintf_legacy_trunc
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsprintf
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _vsnprintf: returns negative if more than count chars would be written; count chars are written
 * note:  Uses options=1 (LEGACY_VSPRINTF_NULL_TERMINATION), the value the UCRT _vsnprintf inline passes; raw -1 vs -2 is not asserted. Only documented parts are asserted; whether a NUL is stored is in ucr_stdio_vsprintf_legacy_nullterm_probe.
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

TC_CASE(overflow_ret)
{
    char b[8]; int n;
    NEED(p_vsprintf);
    memset(b, 'S', sizeof b);
    n = W_sp(1, b, 4, "abcdefgh");
    CHECK(n < 0);
    CHECK_MEM(b, "abc", 3);
    CHECK_EQ_I(b[4], 'S');
}
TC_CASE(exact_fit)
{
    char b[8]; int n;
    NEED(p_vsprintf);
    memset(b, 'S', sizeof b);
    n = W_sp(1, b, 4, "abcd");
    CHECK_EQ_I(n, 4);
    CHECK_MEM(b, "abcd", 4);
    CHECK_EQ_I(b[4], 'S');
}
TC_CASE(no_handler)
{
    char b[4]; int n;
    NEED(p_vsprintf);
    RESET();
    n = W_sp(1, b, 4, "abcdefgh");
    CHECK(n < 0);
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin(); load(); tc_iph_install();
    TC_RUN(overflow_ret); TC_RUN(exact_fit); TC_RUN(no_handler);
    return tc_end();
}

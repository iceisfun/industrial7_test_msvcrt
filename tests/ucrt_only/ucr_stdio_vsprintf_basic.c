/* TC
 * id:    ucr_stdio_vsprintf_basic
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsprintf
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vsprintf, _vsnprintf, snprintf
 * note:  hand-typed prototype; variadic wrappers use va_start to build the va_list
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

TC_CASE(mixed)
{
    char b[64]; int n;
    NEED(p_vsprintf);
    n = W_sp(0, b, sizeof b, "%d %s %c %x %5.2f", -12, "str", 'Z', 0xbeef, 3.14159);
    CHECK_STR(b, "-12 str Z beef  3.14");
    CHECK_EQ_I(n, 20);
}
TC_CASE(empty)
{
    char b[8]; int n;
    NEED(p_vsprintf);
    b[0] = 'x'; b[1] = 'y';
    n = W_sp(0, b, sizeof b, "");
    CHECK_EQ_I(n, 0);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(percent)
{
    char b[16]; int n;
    NEED(p_vsprintf);
    n = W_sp(0, b, sizeof b, "100%%");
    CHECK_STR(b, "100%");
    CHECK_EQ_I(n, 4);
}
TC_CASE(ret_is_strlen)
{
    char b[64]; int n;
    NEED(p_vsprintf);
    n = W_sp(0, b, sizeof b, "%s-%s", "hello", "world");
    CHECK_EQ_I(n, (int)strlen(b));
    CHECK_EQ_I(n, 11);
}
TC_CASE(nul_char)
{
    char b[16]; int n;
    NEED(p_vsprintf);
    memset(b, 'q', sizeof b);
    n = W_sp(0, b, sizeof b, "a%cb", 0);
    CHECK_EQ_I(n, 3);
    CHECK_EQ_I(b[0], 'a'); CHECK_EQ_I(b[1], 0); CHECK_EQ_I(b[2], 'b'); CHECK_EQ_I(b[3], 0);
}

int main(void)
{
    tc_begin(); load(); tc_iph_install();
    TC_RUN(mixed); TC_RUN(empty); TC_RUN(percent); TC_RUN(ret_is_strlen); TC_RUN(nul_char);
    return tc_end();
}

/* TC
 * id:    ucr_neg_stdio_vsprintf_s_buf0_probe
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsprintf_s __stdio_common_vswprintf_s
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs sprintf_s (silent on buffer contents after failure)
 * note:  PROBE: after a failed sprintf_s (buffer too small, NULL format) with a valid buffer of size>0, buffer[0] is set to 0 (UCRT secure printf family terminates the buffer on error). Not documented.
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

TC_CASE(small_narrow)
{
    char b[8]; int n;
    NEED(p_vsprintf_s);
    memset(b, 'S', sizeof b);
    RESET();
    n = W_sps(0, b, 4, "abcdef");
    CHECK(n < 0); CHECK_EQ_I(b[0], 0);
}
TC_CASE(small_wide)
{
    wchar_t b[8]; int n, i;
    NEED(p_vswprintf_s);
    for (i = 0; i < 8; i++) b[i] = L'S';
    RESET();
    n = W_wsps(0, b, 4, L"abcdef");
    CHECK(n < 0); CHECK_EQ_I(b[0], 0);
}
TC_CASE(null_format)
{
    char b[8]; int n;
    NEED(p_vsprintf_s);
    memset(b, 'S', sizeof b);
    RESET();
    n = W_sps(0, b, sizeof b, NULL);
    CHECK(n < 0); CHECK_EQ_I(b[0], 0);
}

int main(void)
{
    tc_begin(); load(); tc_iph_install();
    TC_RUN(small_narrow); TC_RUN(small_wide); TC_RUN(null_format);
    return tc_end();
}

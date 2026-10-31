/* TC
 * id:    scn_vsscanf_s_family
 * area:  scanf
 * funcs:
 * procs: vsscanf_s vswscanf_s __stdio_common_vsscanf __stdio_common_vswscanf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  documented
 * ref:   MS docs vsscanf_s, vswscanf_s (scanf_s family: size argument follows %s/%c/%[ buffer)
 * note:  msvcr120 is not listed: Wine's msvcr120 has vsscanf_s as an unimplemented stub that aborts and hangs the runner (real
 *        msvcr120 probably exports it; the TC_CRT_VER<140 branch resolves it by GetProcAddress). In ucrtbase they are inline over __stdio_common_vsscanf(SECURECRT=1) and
 *        __stdio_common_vswscanf(SECURECRT|LEGACY_WIDE_SPECIFIERS = 3). Cases skip if the symbol is absent.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

typedef int (__cdecl *vss_t)(const char *, const char *, va_list);
typedef int (__cdecl *vsw_t)(const wchar_t *, const wchar_t *, va_list);
typedef int (__cdecl *cn_t)(unsigned long long, const char *, size_t, const char *, void *, va_list);
typedef int (__cdecl *cw_t)(unsigned long long, const wchar_t *, size_t, const wchar_t *, void *, va_list);
static vss_t p_vss; static vsw_t p_vsw; static cn_t p_cn; static cw_t p_cw;

static int callN(const char *s, const char *f, ...)
{
    va_list ap; int r; va_start(ap, f);
#if TC_CRT_VER >= 140
    r = p_cn(1, s, (size_t)-1, f, NULL, ap);
#else
    r = p_vss(s, f, ap);
#endif
    va_end(ap); return r;
}
static int callW(const wchar_t *s, const wchar_t *f, ...)
{
    va_list ap; int r; va_start(ap, f);
#if TC_CRT_VER >= 140
    r = p_cw(3, s, (size_t)-1, f, NULL, ap);
#else
    r = p_vsw(s, f, ap);
#endif
    va_end(ap); return r;
}
static int have_n(void) { return TC_CRT_VER >= 140 ? p_cn != NULL : p_vss != NULL; }
static int have_w(void) { return TC_CRT_VER >= 140 ? p_cw != NULL : p_vsw != NULL; }
static const char *volatile vin;

TC_CASE(s_with_size)
{
    char b[8]; int i, n, v = 0;
    if (!have_n()) TC_SKIP("vsscanf_s not available");
    for (i = 0; i < 8; i++) b[i] = 'Z';
    vin = "9 hello";
    n = callN((const char *)vin, "%d %s", &v, b, (unsigned)8);
    CHECK_EQ_I(n, 2); CHECK_EQ_I(v, 9); CHECK_STR(b, "hello");
}
TC_CASE(c_with_size)
{
    char b[4]; int n;
    if (!have_n()) TC_SKIP("vsscanf_s not available");
    b[0] = b[1] = b[2] = b[3] = 'Z'; vin = "ab";
    n = callN((const char *)vin, "%2c", b, (unsigned)2);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(b[0], 'a'); CHECK_EQ_I(b[1], 'b'); CHECK_EQ_I(b[2], 'Z');
}
TC_CASE(wide_s_with_size)
{
    wchar_t b[8]; int n, v = 0;
    if (!have_w()) TC_SKIP("vswscanf_s not available");
    n = callW(L"4 wide", L"%d %s", &v, b, (unsigned)8);
    CHECK_EQ_I(n, 2); CHECK_EQ_I(v, 4); CHECK_WSTR(b, L"wide");
}
int main(void)
{
    tc_begin();
    p_vss = (vss_t)tc_crt_proc("vsscanf_s"); p_vsw = (vsw_t)tc_crt_proc("vswscanf_s");
    p_cn = (cn_t)tc_crt_proc("__stdio_common_vsscanf"); p_cw = (cw_t)tc_crt_proc("__stdio_common_vswscanf");
    TC_RUN(s_with_size); TC_RUN(c_with_size); TC_RUN(wide_s_with_size);
    return tc_end();
}

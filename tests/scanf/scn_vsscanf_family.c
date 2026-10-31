/* TC
 * id:    scn_vsscanf_family
 * area:  scanf
 * funcs:
 * procs: vsscanf vswscanf __stdio_common_vsscanf __stdio_common_vswscanf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.21.6.14 vsscanf, 7.29.2.8 vswscanf; MS docs vsscanf, vswscanf (C99 additions, VS2013+)
 * note:  msvcr120 exports vsscanf/vswscanf and they are called directly. In ucrtbase they are inline in the MS headers over
 *        __stdio_common_vsscanf/__stdio_common_vswscanf(options 0 narrow, legacy wide specifiers 2 for wide), so the
 *        wrapper here forwards to those. Cases skip if the needed symbol is absent.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

typedef int (__cdecl *vss_t)(const char *, const char *, va_list);
typedef int (__cdecl *vsw_t)(const wchar_t *, const wchar_t *, va_list);
typedef int (__cdecl *cn_t)(unsigned long long, const char *, size_t, const char *, void *, va_list);
typedef int (__cdecl *cw_t)(unsigned long long, const wchar_t *, size_t, const wchar_t *, void *, va_list);
static vss_t p_vss; static vsw_t p_vsw; static cn_t p_cn; static cw_t p_cw;

static int vn(const char *s, const char *f, va_list ap)
{
#if TC_CRT_VER >= 140
    return p_cn(0, s, (size_t)-1, f, NULL, ap);
#else
    return p_vss(s, f, ap);
#endif
}
static int vw(const wchar_t *s, const wchar_t *f, va_list ap)
{
#if TC_CRT_VER >= 140
    return p_cw(2, s, (size_t)-1, f, NULL, ap);
#else
    return p_vsw(s, f, ap);
#endif
}
static int callN(const char *s, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = vn(s, f, ap); va_end(ap); return r; }
static int callW(const wchar_t *s, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = vw(s, f, ap); va_end(ap); return r; }
static int have_n(void) { return TC_CRT_VER >= 140 ? p_cn != NULL : p_vss != NULL; }
static int have_w(void) { return TC_CRT_VER >= 140 ? p_cw != NULL : p_vsw != NULL; }
static const char *volatile vin;

TC_CASE(ints)
{
    int a = 0, b = 0, n;
    if (!have_n()) TC_SKIP("vsscanf not available");
    vin = "10 -20";
    n = callN((const char *)vin, "%d %d", &a, &b);
    CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 10); CHECK_EQ_I(b, -20);
}
TC_CASE(strings)
{
    char s[16], t[16]; int n;
    if (!have_n()) TC_SKIP("vsscanf not available");
    vin = "alpha beta";
    n = callN((const char *)vin, "%s %s", s, t);
    CHECK_EQ_I(n, 2); CHECK_STR(s, "alpha"); CHECK_STR(t, "beta");
}
TC_CASE(partial)
{
    int a = 0, b = 77, n;
    if (!have_n()) TC_SKIP("vsscanf not available");
    vin = "5 x";
    n = callN((const char *)vin, "%d %d", &a, &b);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 5); CHECK_EQ_I(b, 77);
}
TC_CASE(ret_eof)
{
    int a = 3, n;
    if (!have_n()) TC_SKIP("vsscanf not available");
    vin = "";
    n = callN((const char *)vin, "%d", &a);
    CHECK_EQ_I(n, EOF); CHECK_EQ_I(a, 3);
}
TC_CASE(wide)
{
    int a = 0, n; wchar_t w[16];
    if (!have_w()) TC_SKIP("vswscanf not available");
    n = callW(L"31 wide", L"%d %ls", &a, w);
    CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 31); CHECK_WSTR(w, L"wide");
}
TC_CASE(wide_eof)
{
    int a = 3, n;
    if (!have_w()) TC_SKIP("vswscanf not available");
    n = callW(L"", L"%d", &a);
    CHECK_EQ_I(n, EOF); CHECK_EQ_I(a, 3);
}
int main(void)
{
    tc_begin();
    p_vss = (vss_t)tc_crt_proc("vsscanf"); p_vsw = (vsw_t)tc_crt_proc("vswscanf");
    p_cn = (cn_t)tc_crt_proc("__stdio_common_vsscanf"); p_cw = (cw_t)tc_crt_proc("__stdio_common_vswscanf");
    TC_RUN(ints); TC_RUN(strings); TC_RUN(partial); TC_RUN(ret_eof); TC_RUN(wide); TC_RUN(wide_eof);
    return tc_end();
}

/* TC
 * id:    ucr_stdio_vswscanf_basic
 * area:  ucrt_only
 * funcs: fopen fclose remove fputs
 * procs: __stdio_common_vswscanf __stdio_common_vfwscanf
 * crt:   win10
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.2.4 (swscanf), 7.29.2.2 (fwscanf); MS docs swscanf, scanf width specification (%hs narrow, %ls wide)
 * note:  Back ends called directly with options 0 (conformant wide specifiers).
 */
#include "tc.h"
#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

typedef int (__cdecl *vswscanf_t)(unsigned long long, const wchar_t *, size_t, const wchar_t *, void *, va_list);
typedef int (__cdecl *vfwscanf_t)(unsigned long long, FILE *, const wchar_t *, void *, va_list);
static vswscanf_t p_vsw; static vfwscanf_t p_vfw;

static int xw(const wchar_t *in, const wchar_t *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt); r = p_vsw(0, in, (size_t)-1, fmt, NULL, ap); va_end(ap);
    return r;
}
static int xf(FILE *f, const wchar_t *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt); r = p_vfw(0, f, fmt, NULL, ap); va_end(ap);
    return r;
}

TC_CASE(int_str_float)
{
    int n = 0; wchar_t w[16]; float f = 0; int r;
    r = xw(L"12 word 2.5", L"%d %ls %f", &n, w, &f);
    CHECK_EQ_I(r, 3); CHECK_EQ_I(n, 12); CHECK_WSTR(w, L"word"); CHECK(f == 2.5f);
}
TC_CASE(lc)
{
    wchar_t c = 0; int r;
    r = xw(L"Qrs", L"%lc", &c);
    CHECK_EQ_I(r, 1); CHECK_EQ_U(c, L'Q');
}
TC_CASE(hs_narrow)
{
    char b[16]; int r;
    r = xw(L"narrow x", L"%hs", b);
    CHECK_EQ_I(r, 1); CHECK_STR(b, "narrow");
}
TC_CASE(eof_empty)
{
    int n = 0; int r;
    r = xw(L"", L"%d", &n);
    CHECK_EQ_I(r, -1);
}
TC_CASE(file_ascii)
{
    const char *fn = "tc_ucr_stdio_vswscanf_basic_f.tmp";
    FILE *f; int n = 0; wchar_t w[16]; int r;
    f = fopen(fn, "w"); CHECK(f != NULL); if (!f) return;
    fputs("42 hello\n", f); fclose(f);
    f = fopen(fn, "r"); CHECK(f != NULL);
    if (f) { r = xf(f, L"%d %ls", &n, w); CHECK_EQ_I(r, 2); CHECK_EQ_I(n, 42); CHECK_WSTR(w, L"hello"); fclose(f); }
    remove(fn);
}

int main(void)
{
    tc_begin();
    p_vsw = (vswscanf_t)tc_crt_proc("__stdio_common_vswscanf");
    p_vfw = (vfwscanf_t)tc_crt_proc("__stdio_common_vfwscanf");
    if (!p_vsw || !p_vfw) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(int_str_float); TC_RUN(lc); TC_RUN(hs_narrow); TC_RUN(eof_empty); TC_RUN(file_ascii);
    return tc_end();
}

/* TC
 * id:    pfw_neg_swprintf_s_toosmall
 * area:  stdio_printf
 * funcs: 
 * procs: swprintf_s vswprintf_s __stdio_common_vswprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s, swprintf_s: buffer too small -> invalid parameter handler, returns -1, buffer[0]=0, errno ERANGE
 * note:  ucrtbase: resolved via the __stdio_common_vswprintf_s export (the wchar.h inline functions call it).
 */
#include "tc.h"
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
/* ucrtbase has swprintf_s/vswprintf_s only inline (wchar.h) over __stdio_common_vswprintf_s */
typedef int (__cdecl *common_t)(unsigned __int64, wchar_t *, size_t, const wchar_t *, void *, va_list);
static common_t p_common;
static int x_vswprintf_s(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return p_common(0, b, n, f, NULL, ap); }
#define LOAD_VSWS() ((p_common = (common_t)tc_crt_proc("__stdio_common_vswprintf_s")) != NULL)
#else
typedef int (__cdecl *vsws_t)(wchar_t *, size_t, const wchar_t *, va_list);
static vsws_t p_vsws;
static int x_vswprintf_s(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return p_vsws(b, n, f, ap); }
#define LOAD_VSWS() ((p_vsws = (vsws_t)tc_crt_proc("vswprintf_s")) != NULL)
#endif
static int vsw(wchar_t *b, size_t n, const wchar_t *f, ...)
{ int r; va_list ap; va_start(ap, f); r = x_vswprintf_s(b, n, f, ap); va_end(ap); return r; }
#if TC_CRT_VER >= 140
static int x_swprintf_s(wchar_t *b, size_t n, const wchar_t *f, ...)
{ int r; va_list ap; va_start(ap, f); r = x_vswprintf_s(b, n, f, ap); va_end(ap); return r; }
#define LOAD_SWS() LOAD_VSWS()
#else
TC_DECL_PROC(int, swprintf_s, (wchar_t *, size_t, const wchar_t *, ...));
#define x_swprintf_s tc_p_swprintf_s
#define LOAD_SWS() (TC_LOAD_PROC(swprintf_s) != NULL)
#endif
static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }
static const wchar_t * volatile vfmt = L"%d";
static const wchar_t * volatile vnull_fmt;
static wchar_t * volatile vnull_buf;
static const wchar_t * volatile vabcd = L"abcd";

TC_CASE(control) { wchar_t b[16]; int n = x_swprintf_s(b, 6, vfmt, 12345); CHECK_EQ_I(n, 5); CHECK_WSTR(b, L"12345"); }
TC_CASE(ret_neg1_buf0_NUL_ERANGE)
{
    wchar_t b[16]; int n; fill(b); tc_iph_count = 0; errno = 0;
    n = x_swprintf_s(b, 5, vfmt, 12345);   /* needs 6 wchar_t */
    CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(iph_count_1_each) { wchar_t b[16]; tc_iph_count = 0; x_swprintf_s(b, 2, vfmt, 99); CHECK_EQ_I(tc_iph_count, 1); x_swprintf_s(b, 2, vfmt, 99); CHECK_EQ_I(tc_iph_count, 2); }
TC_CASE(exact_fit_len_eq_size_fails)
{
    wchar_t b[16]; int n; fill(b); tc_iph_count = 0; errno = 0;
    n = x_swprintf_s(b, 4, L"%ls", vabcd);   /* 4 chars + NUL needs size 5 */
    CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    fill(b); n = x_swprintf_s(b, 5, L"%ls", vabcd); CHECK_EQ_I(n, 4); CHECK_WSTR(b, L"abcd");
}
TC_CASE(vswprintf_s_same)
{
    wchar_t b[16]; int n; fill(b); tc_iph_count = 0; errno = 0;
    n = vsw(b, 5, vfmt, 12345);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); CHECK(LOAD_VSWS()); CHECK(LOAD_SWS());
  TC_RUN(control); TC_RUN(ret_neg1_buf0_NUL_ERANGE); TC_RUN(iph_count_1_each); TC_RUN(exact_fit_len_eq_size_fails); TC_RUN(vswprintf_s_same); return tc_end(); }

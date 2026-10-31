/* TC
 * id:    pfw_neg_swprintf_s_null_args
 * area:  stdio_printf
 * funcs: 
 * procs: swprintf_s vswprintf_s __stdio_common_vswprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs swprintf_s: NULL buffer, NULL format or zero size -> invalid parameter handler, returns -1, errno EINVAL
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
static const wchar_t * volatile vfmt = L"%d";
static const wchar_t * volatile vnull_fmt;
static wchar_t * volatile vnull_buf;

TC_CASE(control) { wchar_t b[8]; CHECK_EQ_I(x_swprintf_s(b, 8, vfmt, 5), 1); }
TC_CASE(null_buf_EINVAL) { int n; tc_iph_count = 0; errno = 0; n = x_swprintf_s(vnull_buf, 8, vfmt, 5); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_fmt_EINVAL) { wchar_t b[8]; int n; b[0] = L'Z'; tc_iph_count = 0; errno = 0; n = x_swprintf_s(b, 8, vnull_fmt); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size0_EINVAL) { wchar_t b[8]; int n; tc_iph_count = 0; errno = 0; n = x_swprintf_s(b, 0, vfmt, 5); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(iph_counts_3)
{
    wchar_t b[8]; tc_iph_count = 0;
    CHECK_EQ_I(x_swprintf_s(vnull_buf, 8, vfmt, 1), -1);
    CHECK_EQ_I(x_swprintf_s(b, 8, vnull_fmt), -1);
    CHECK_EQ_I(x_swprintf_s(b, 0, vfmt, 1), -1);
    CHECK_EQ_I(tc_iph_count, 3);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); CHECK(LOAD_VSWS()); CHECK(LOAD_SWS());
  TC_RUN(control); TC_RUN(null_buf_EINVAL); TC_RUN(null_fmt_EINVAL); TC_RUN(size0_EINVAL); TC_RUN(iph_counts_3); return tc_end(); }

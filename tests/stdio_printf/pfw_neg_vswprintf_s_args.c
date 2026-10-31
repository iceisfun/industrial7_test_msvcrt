/* TC
 * id:    pfw_neg_vswprintf_s_args
 * area:  stdio_printf
 * funcs: 
 * procs: swprintf_s vswprintf_s __stdio_common_vswprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs vswprintf_s, _vswprintf_s_l: NULL buffer/format or zero size -> EINVAL; too small -> ERANGE with buffer[0]=0
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
static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }
static const wchar_t * volatile vfmt = L"%d";
static const wchar_t * volatile vnull_fmt;
static wchar_t * volatile vnull_buf;

TC_CASE(control) { wchar_t b[8]; CHECK_EQ_I(vsw(b, 8, vfmt, 5), 1); CHECK_WSTR(b, L"5"); }
TC_CASE(null_buf) { int n; tc_iph_count = 0; errno = 0; n = vsw(vnull_buf, 8, vfmt, 5); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_fmt) { wchar_t b[8]; int n; tc_iph_count = 0; errno = 0; n = vsw(b, 8, vnull_fmt); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(zero_size) { wchar_t b[8]; int n; tc_iph_count = 0; errno = 0; n = vsw(b, 0, vfmt, 5); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(overflow_ERANGE) { wchar_t b[16]; int n; fill(b); tc_iph_count = 0; errno = 0; n = vsw(b, 3, vfmt, 12345); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); CHECK(LOAD_VSWS());
  TC_RUN(control); TC_RUN(null_buf); TC_RUN(null_fmt); TC_RUN(zero_size); TC_RUN(overflow_ERANGE); return tc_end(); }

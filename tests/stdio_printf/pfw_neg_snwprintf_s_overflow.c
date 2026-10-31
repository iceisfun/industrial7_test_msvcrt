/* TC
 * id:    pfw_neg_snwprintf_s_overflow
 * area:  stdio_printf
 * funcs: 
 * procs: _snwprintf_s _vsnwprintf_s __stdio_common_vsnwprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf_s: count >= size (not _TRUNCATE) and output does not fit -> invalid parameter handler, returns -1, buffer[0]=0, errno ERANGE; NULL buffer -> EINVAL
 */
#include "tc.h"
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
typedef int (__cdecl *snw_common_t)(unsigned __int64, wchar_t *, size_t, size_t, const wchar_t *, void *, va_list);
static snw_common_t p_snw_common;
static int x_vsnwprintf_s(wchar_t *b, size_t sz, size_t cnt, const wchar_t *f, va_list ap) { return p_snw_common(0, b, sz, cnt, f, NULL, ap); }
#define LOAD_SNWS() ((p_snw_common = (snw_common_t)tc_crt_proc("__stdio_common_vsnwprintf_s")) != NULL)
#else
typedef int (__cdecl *vsnws_t)(wchar_t *, size_t, size_t, const wchar_t *, va_list);
static vsnws_t p_vsnws;
static int x_vsnwprintf_s(wchar_t *b, size_t sz, size_t cnt, const wchar_t *f, va_list ap) { return p_vsnws(b, sz, cnt, f, ap); }
#define LOAD_SNWS() ((p_vsnws = (vsnws_t)tc_crt_proc("_vsnwprintf_s")) != NULL)
#endif
static int x_snwprintf_s(wchar_t *b, size_t sz, size_t cnt, const wchar_t *f, ...)
{ int r; va_list ap; va_start(ap, f); r = x_vsnwprintf_s(b, sz, cnt, f, ap); va_end(ap); return r; }
static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }
static const wchar_t * volatile vfmt = L"%d";
static const wchar_t * volatile vnull_fmt;
static wchar_t * volatile vnull_buf;

TC_CASE(control) { wchar_t b[16]; CHECK_EQ_I(x_snwprintf_s(b, 6, 5, vfmt, 12345), 5); CHECK_WSTR(b, L"12345"); }
TC_CASE(overflow)
{
    wchar_t b[16]; int n; fill(b); tc_iph_count = 0; errno = 0;
    n = x_snwprintf_s(b, 4, 4, vfmt, 12345);   /* 5 chars need size 6 */
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(count_gt_size_overflow)
{
    wchar_t b[16]; int n; fill(b); tc_iph_count = 0; errno = 0;
    n = x_snwprintf_s(b, 4, 10, vfmt, 12345);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(null_buffer_EINVAL) { int n; tc_iph_count = 0; errno = 0; n = x_snwprintf_s(vnull_buf, 8, 4, vfmt, 1); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); CHECK(LOAD_SNWS());
  TC_RUN(control); TC_RUN(overflow); TC_RUN(count_gt_size_overflow); TC_RUN(null_buffer_EINVAL); return tc_end(); }

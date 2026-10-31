/* TC
 * id:    pfw_neg_snwprintf_s_truncate
 * area:  stdio_printf
 * funcs: 
 * procs: _snwprintf_s _vsnwprintf_s __stdio_common_vsnwprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf_s, _snwprintf_s, _vsnwprintf_s: count == _TRUNCATE truncates, returns -1, no invalid parameter handler; count != _TRUNCATE and result does not fit size, or NULL args: handler invoked (EINVAL for NULL format), returns -1
 * note:  count < len truncation (non-_TRUNCATE) is in pfw_probe_snwprintf_s_count_lt_len. ucrtbase: resolved via the __stdio_common_vsnwprintf_s export.
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
#define TRUNC ((size_t)-1)
static int vsnws(wchar_t *b, size_t sz, size_t cnt, const wchar_t *f, ...)
{ int r; va_list ap; va_start(ap, f); r = x_vsnwprintf_s(b, sz, cnt, f, ap); va_end(ap); return r; }
static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }
static const wchar_t * volatile vfmt = L"%d";
static const wchar_t * volatile vnull_fmt;

TC_CASE(control_fits) { wchar_t b[16]; int n = x_snwprintf_s(b, 4, TRUNC, vfmt, 123); CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"123"); }
TC_CASE(truncate_ret_neg1)
{
    wchar_t b[16]; int n; fill(b); tc_iph_count = 0; 
    n = x_snwprintf_s(b, 4, TRUNC, vfmt, 123456);
    CHECK_EQ_I(n, -1);
}
TC_CASE(buffer_NUL_terminated_prefix) { wchar_t b[16]; int n; fill(b); n = x_snwprintf_s(b, 4, TRUNC, vfmt, 123456); CHECK_EQ_I(n, -1); CHECK_WSTR(b, L"123"); CHECK(b[4] == L'Z'); }
TC_CASE(no_iph) { wchar_t b[16]; tc_iph_count = 0; x_snwprintf_s(b, 4, TRUNC, vfmt, 123456); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(vsnwprintf_s_same)
{
    wchar_t b[16]; int n; fill(b); tc_iph_count = 0;
    n = vsnws(b, 4, TRUNC, vfmt, 123456);
    CHECK_EQ_I(n, -1); CHECK_WSTR(b, L"123"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(overflow_without_truncate_calls_handler)
{
    /* count (10) is not _TRUNCATE and the 6-char result plus NUL does not fit in size 4: invalid parameter handler, -1 */
    wchar_t b[16]; int n; fill(b); tc_iph_count = 0;
    n = x_snwprintf_s(b, 4, 10, vfmt, 123456);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format_calls_handler)
{
    wchar_t b[16]; int n; fill(b); tc_iph_count = 0; errno = 0;
    n = x_snwprintf_s(b, 16, 8, vnull_fmt);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); CHECK(LOAD_SNWS());
  TC_RUN(control_fits); TC_RUN(truncate_ret_neg1); TC_RUN(buffer_NUL_terminated_prefix); TC_RUN(no_iph); TC_RUN(vsnwprintf_s_same); TC_RUN(overflow_without_truncate_calls_handler); TC_RUN(null_format_calls_handler); return tc_end(); }

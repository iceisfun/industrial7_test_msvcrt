/* TC
 * id:    pfw_neg_vsnwprintf_null_args
 * area:  stdio_printf
 * funcs: _vsnwprintf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf, _snwprintf, _vsnprintf, _vsnwprintf: NULL buffer with nonzero count, or NULL format -> invalid parameter handler, returns -1, errno EINVAL
 */
#include "tc.h"
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

static int vw(wchar_t *b, size_t n, const wchar_t *f, ...)
{ int r; va_list ap; va_start(ap, f); r = _vsnwprintf(b, n, f, ap); va_end(ap); return r; }
static wchar_t * volatile vnull_buf;
static const wchar_t * volatile vnull_fmt;
static const wchar_t * volatile vfmt = L"%d";

TC_CASE(control) { wchar_t b[16]; int n = vw(b, 16, vfmt, 7); CHECK_EQ_I(n, 1); CHECK_WSTR(b, L"7"); }
TC_CASE(null_buf) { int n; tc_iph_count = 0; errno = 0; n = vw(vnull_buf, 10, vfmt, 5); CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(null_fmt) { wchar_t b[8]; int n; b[0] = L'Z'; tc_iph_count = 0; errno = 0; n = vw(b, 8, vnull_fmt, 5); CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(EINVAL_both) { int n; tc_iph_count = 0; errno = 0; n = vw(vnull_buf, 10, vnull_fmt, 5); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(iph_counts)
{
    wchar_t b[8]; tc_iph_count = 0;
    vw(vnull_buf, 10, vfmt, 1); CHECK_EQ_I(tc_iph_count, 1);
    vw(b, 8, vnull_fmt, 1);     CHECK_EQ_I(tc_iph_count, 2);
    vw(b, 8, vfmt, 1);          CHECK_EQ_I(tc_iph_count, 2);  /* valid call must not trigger the handler */
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(control); TC_RUN(null_buf); TC_RUN(null_fmt); TC_RUN(EINVAL_both); TC_RUN(iph_counts); return tc_end(); }

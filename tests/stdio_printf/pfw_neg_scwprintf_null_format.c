/* TC
 * id:    pfw_neg_scwprintf_null_format
 * area:  stdio_printf
 * funcs: _scwprintf _vscwprintf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _scprintf, _scwprintf, _vscprintf, _vscwprintf: NULL format -> invalid parameter handler, returns -1, errno EINVAL
 */
#include "tc.h"
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

static int vscw(const wchar_t *f, ...)
{ int r; va_list ap; va_start(ap, f); r = _vscwprintf(f, ap); va_end(ap); return r; }
static const wchar_t * volatile vnull_fmt;
static const wchar_t * volatile vfmt = L"%d";

TC_CASE(control) { CHECK_EQ_I(_scwprintf(vfmt, 12345), 5); CHECK_EQ_I(vscw(vfmt, 12345), 5); }
TC_CASE(scwprintf_neg1_EINVAL_iph) { int n; tc_iph_count = 0; errno = 0; n = _scwprintf(vnull_fmt); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(vscwprintf_neg1_EINVAL_iph) { int n; tc_iph_count = 0; errno = 0; n = vscw(vnull_fmt); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(control); TC_RUN(scwprintf_neg1_EINVAL_iph); TC_RUN(vscwprintf_neg1_EINVAL_iph); return tc_end(); }

/* TC
 * id:    pfw_neg_swprintf_c99_null_args
 * area:  stdio_printf
 * funcs: swprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs swprintf, _swprintf, _swprintf_l (C99 form with count): NULL buffer (count != 0) or NULL format -> invalid parameter handler, returns -1, errno EINVAL
 */
#include "tc.h"
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

static wchar_t * volatile vnull_buf;
static const wchar_t * volatile vnull_fmt;
static const wchar_t * volatile vfmt = L"%d";

TC_CASE(control) { wchar_t b[8]; CHECK_EQ_I(swprintf(b, 8, vfmt, 42), 2); CHECK_WSTR(b, L"42"); }
TC_CASE(null_buffer_count_nonzero) { int n; tc_iph_count = 0; errno = 0; n = swprintf(vnull_buf, 8, vfmt, 1); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_format) { wchar_t b[8]; int n; b[0] = L'Z'; tc_iph_count = 0; errno = 0; n = swprintf(b, 8, vnull_fmt); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(EINVAL_iph_counts) { wchar_t b[8]; tc_iph_count = 0; swprintf(vnull_buf, 8, vfmt, 1); swprintf(b, 8, vnull_fmt); CHECK_EQ_I(tc_iph_count, 2); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(control); TC_RUN(null_buffer_count_nonzero); TC_RUN(null_format); TC_RUN(EINVAL_iph_counts); return tc_end(); }

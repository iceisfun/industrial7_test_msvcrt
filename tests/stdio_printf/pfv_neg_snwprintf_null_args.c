/* TC
 * id:    pfv_neg_snwprintf_null_args
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snwprintf: NULL buffer (count > 0) or NULL format -> invalid parameter handler, -1, EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static wchar_t *volatile nullbuf = NULL;
static const wchar_t *volatile nullfmt = NULL;
TC_CASE(null_buf) { int n; errno = 0; tc_iph_count = 0; n = _snwprintf(nullbuf, 8, L"abc"); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_fmt) { wchar_t b[8]; int n; errno = 0; tc_iph_count = 0; n = _snwprintf(b, 8, nullfmt); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_buf); TC_RUN(null_fmt); return tc_end(); }

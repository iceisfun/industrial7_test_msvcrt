/* TC
 * id:    pfv_neg_snprintf_s_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s: NULL buffer/format or zero sizeOfBuffer -> invalid parameter handler, -1, EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static char b[16];
TC_CASE(null_buf) { char *volatile p = NULL; int n; tc_iph_count = 0; errno = 0; n = _snprintf_s(p, 8, 4, "a"); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_fmt) { const char *volatile f = NULL; int n; tc_iph_count = 0; errno = 0; n = _snprintf_s(b, 8, 4, f); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size_zero) { volatile size_t sz = 0; int n; tc_iph_count = 0; errno = 0; n = _snprintf_s(b, sz, 4, "a"); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_buf); TC_RUN(null_fmt); TC_RUN(size_zero); return tc_end(); }

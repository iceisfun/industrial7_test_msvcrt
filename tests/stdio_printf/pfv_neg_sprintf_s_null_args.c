/* TC
 * id:    pfv_neg_sprintf_s_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s: NULL buffer/format or size 0 -> invalid parameter handler, returns -1, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static char b[16];
TC_CASE(null_buf_size8) { char *volatile p = NULL; int n; tc_iph_count = 0; errno = 0; n = sprintf_s(p, 8, "a"); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_fmt) { const char *volatile f = NULL; int n; tc_iph_count = 0; errno = 0; n = sprintf_s(b, 8, f); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(zero_size_nonnull) { volatile size_t sz = 0; int n; tc_iph_count = 0; errno = 0; n = sprintf_s(b, sz, "a"); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_buf_size0) { char *volatile p = NULL; volatile size_t sz = 0; int n; tc_iph_count = 0; n = sprintf_s(p, sz, "a"); CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_buf_size8); TC_RUN(null_fmt); TC_RUN(zero_size_nonnull); TC_RUN(null_buf_size0); return tc_end(); }

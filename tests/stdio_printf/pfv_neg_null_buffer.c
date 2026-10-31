/* TC
 * id:    pfv_neg_null_buffer
 * area:  stdio_printf
 * funcs: sprintf _snprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf, _snprintf: NULL buffer (count > 0) -> invalid parameter handler, returns -1, errno EINVAL
 * note:  _snprintf(NULL,0) is deliberately not tested here.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static char *volatile nullbuf = NULL;
TC_CASE(sprintf_null_buf) { int n; errno = 0; tc_iph_count = 0; n = sprintf(nullbuf, "abc"); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(snprintf_null_buf_nonzero_count) { int n; errno = 0; tc_iph_count = 0; n = _snprintf(nullbuf, 8, "abc"); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(sprintf_null_buf); TC_RUN(snprintf_null_buf_nonzero_count); return tc_end(); }

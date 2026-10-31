/* TC
 * id:    pfv_neg_null_format
 * area:  stdio_printf
 * funcs: sprintf _snprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf, _snprintf: NULL format -> invalid parameter handler, returns -1, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static const char *volatile nullfmt = NULL;
TC_CASE(sprintf_null_fmt) { char b[8] = "x"; int n; errno = 0; tc_iph_count = 0; n = sprintf(b, nullfmt); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(snprintf_null_fmt) { char b[8] = "x"; int n; errno = 0; tc_iph_count = 0; n = _snprintf(b, sizeof b, nullfmt); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(iph_count_per_call) { char b[8]; tc_iph_count = 0; sprintf(b, nullfmt); sprintf(b, nullfmt); _snprintf(b, sizeof b, nullfmt); CHECK_EQ_I(tc_iph_count, 3); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(sprintf_null_fmt); TC_RUN(snprintf_null_fmt); TC_RUN(iph_count_per_call); return tc_end(); }

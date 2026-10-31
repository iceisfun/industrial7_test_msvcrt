/* TC
 * id:    pfv_neg_scprintf_null_format
 * area:  stdio_printf
 * funcs: _scprintf _scwprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _scprintf, _scwprintf: NULL format -> invalid parameter handler, -1, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static const char *volatile nullfmt = NULL;
static const wchar_t *volatile nullwfmt = NULL;
TC_CASE(scprintf_null) { int n; errno = 0; tc_iph_count = 0; n = _scprintf(nullfmt); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(scwprintf_null) { int n; errno = 0; tc_iph_count = 0; n = _scwprintf(nullwfmt); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(scprintf_null); TC_RUN(scwprintf_null); return tc_end(); }

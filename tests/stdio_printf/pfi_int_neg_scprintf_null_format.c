/* TC
 * id:    pfi_int_neg_scprintf_null_format
 * area:  stdio_printf
 * funcs: _scprintf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _scprintf: NULL format -> invalid parameter handler, returns -1, errno EINVAL (parameter validation, VS2005+)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(null_fmt) { int n; volatile const char *f = NULL; tc_iph_count = 0; errno = 0; n = _scprintf((const char *)f); CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_fmt); return tc_end(); }

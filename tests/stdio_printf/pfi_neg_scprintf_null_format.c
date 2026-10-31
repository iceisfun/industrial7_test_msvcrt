/* TC
 * id:    pfi_neg_scprintf_null_format
 * area:  stdio_printf
 * funcs: _scprintf
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _scprintf: NULL format -> invalid parameter handler, -1, EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
TC_CASE(null_format) {
    const char *volatile f = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = _scprintf(f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_format);
    return tc_end();
}

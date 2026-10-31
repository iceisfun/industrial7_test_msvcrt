/* TC
 * id:    pfi_neg_vscprintf_null_format
 * area:  stdio_printf
 * funcs: _vscprintf
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _vscprintf: NULL format -> invalid parameter handler, -1, EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
static int w_vscprintf(const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = _vscprintf(f, ap); va_end(ap); return r;
}
TC_CASE(null_format) {
    const char *volatile f = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = w_vscprintf(f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_format);
    return tc_end();
}

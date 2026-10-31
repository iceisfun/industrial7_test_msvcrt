/* TC
 * id:    pfi_neg_vsprintf_null_buffer
 * area:  stdio_printf
 * funcs: vsprintf
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs vsprintf: NULL buffer or format -> invalid parameter handler, -1, EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
static int w_vsprintf(char *b, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = vsprintf(b, f, ap); va_end(ap); return r;
}
TC_CASE(null_buffer) {
    char *volatile p = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = w_vsprintf(p, "x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format) {
    char b[8]; const char *volatile f = NULL; int r;
    b[0] = 'Q';
    tc_iph_count = 0; errno = 0;
    r = w_vsprintf(b, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_buffer); TC_RUN(null_format);
    return tc_end();
}

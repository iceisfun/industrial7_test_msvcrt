/* TC
 * id:    pfi_neg_snwprintf_null_args
 * area:  stdio_printf
 * funcs: _snwprintf
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf, _snwprintf: NULL buffer (count>0) or format -> invalid parameter handler, -1, EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
TC_CASE(null_buffer) {
    wchar_t *volatile p = NULL; volatile size_t cnt = 4; int r;
    tc_iph_count = 0; errno = 0;
    r = _snwprintf(p, cnt, L"%d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format) {
    wchar_t b[8]; const wchar_t *volatile f = NULL; volatile size_t cnt = 4; int r;
    b[0] = L'Q';
    tc_iph_count = 0; errno = 0;
    r = _snwprintf(b, cnt, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_buffer); TC_RUN(null_format);
    return tc_end();
}

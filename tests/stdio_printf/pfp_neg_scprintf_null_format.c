/* TC
 * id:    pfp_neg_scprintf_null_format
 * area:  stdio_printf
 * funcs: _scprintf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _scprintf, Parameter validation: NULL format -> handler, returns -1, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>
#include <stdarg.h>

TC_CASE(null_format)
{
    const char *volatile f = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = _scprintf(f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(valid_control)
{
    volatile void *p = (void *)(size_t)0xABC; int r;
    tc_iph_count = 0;
    r = _scprintf("%p", p);
    CHECK_EQ_I(r, (int)(sizeof(void *) * 2));
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_format); TC_RUN(valid_control); return tc_end(); }

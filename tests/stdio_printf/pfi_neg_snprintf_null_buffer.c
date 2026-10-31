/* TC
 * id:    pfi_neg_snprintf_null_buffer
 * area:  stdio_printf
 * funcs: _snprintf
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf (VS2005+ parameter validation)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(null_buffer)
{
    char *volatile p = NULL; int n; volatile int v = 1;
    if (!tc_iph_install()) TC_SKIP("no handler api");
    errno = 0; tc_iph_count = 0;
    n = _snprintf(p, 10, "%d", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void) { tc_begin(); TC_RUN(null_buffer); return tc_end(); }

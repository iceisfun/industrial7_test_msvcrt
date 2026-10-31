/* TC
 * id:    pfi_neg_snprintf_null_format
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

TC_CASE(null_format)
{
    char b[8]; const char *volatile f = NULL; int n;
    if (!tc_iph_install()) TC_SKIP("no handler api");
    b[0] = 'x'; errno = 0; tc_iph_count = 0;
    n = _snprintf(b, sizeof b, f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void) { tc_begin(); TC_RUN(null_format); return tc_end(); }

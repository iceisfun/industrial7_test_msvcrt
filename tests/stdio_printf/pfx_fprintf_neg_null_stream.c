/* TC
 * id:    pfx_fprintf_neg_null_stream
 * area:  stdio_printf
 * funcs: fprintf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fprintf; Parameter validation: stream NULL -> invalid parameter handler, errno EINVAL, returns -1
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(null_stream)
{
    int r; volatile FILE *nf = NULL;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0; errno = 0;
    r = fprintf((FILE *)nf, "abc %d", 1);
    CHECK(r < 0);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
}

int main(void)
{
    tc_begin();
    TC_RUN(null_stream);
    return tc_end();
}

/* TC
 * id:    fio_neg_clearerr_null
 * area:  stdio_file
 * funcs: clearerr
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs clearerr; parameter validation
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>

TC_CASE(null)
{
    FILE *volatile p = NULL;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    clearerr(p);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(null); return tc_end(); }

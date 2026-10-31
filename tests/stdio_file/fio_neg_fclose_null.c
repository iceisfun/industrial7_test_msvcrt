/* TC
 * id:    fio_neg_fclose_null
 * area:  stdio_file
 * funcs: fclose
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fclose; parameter validation
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

TC_CASE(null)
{
    volatile int rc;
    FILE *volatile p = NULL;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    rc = fclose(p);
    CHECK_EQ_I(rc, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(null); return tc_end(); }

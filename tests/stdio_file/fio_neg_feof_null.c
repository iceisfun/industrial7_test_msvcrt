/* TC
 * id:    fio_neg_feof_null
 * area:  stdio_file
 * funcs: feof
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs feof, ferror (parameter validation)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(feof_null)
{
    volatile int rc; FILE *volatile p = NULL;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    rc = feof(p);
    CHECK_EQ_I(rc, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(feof_null); return tc_end(); }

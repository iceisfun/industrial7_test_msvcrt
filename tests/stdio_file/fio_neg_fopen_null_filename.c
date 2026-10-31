/* TC
 * id:    fio_neg_fopen_null_filename
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen
 * procs: 
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen: parameter validation, EINVAL
 * note:  NULL filename invokes the invalid parameter handler and returns NULL/EINVAL.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char * volatile nul_name = NULL;

TC_CASE(null_name)
{
    FILE *f; int n0 = tc_iph_count; errno = 0;
    f = fopen(nul_name, "r");
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - n0, 1);
    if (f) fclose(f);
}
TC_CASE(null_name_write)
{
    FILE *f; int n0 = tc_iph_count; errno = 0;
    f = fopen(nul_name, "w");
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - n0, 1);
    if (f) fclose(f);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(null_name); TC_RUN(null_name_write);
    return tc_end();
}

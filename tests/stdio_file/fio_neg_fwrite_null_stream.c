/* TC
 * id:    fio_neg_fwrite_null_stream
 * area:  stdio_file
 * kind:  negative
 * funcs: fwrite
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fwrite: NULL stream invokes invalid parameter handler, errno EINVAL, returns 0
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static FILE * volatile nul = NULL;

TC_CASE(null_stream)
{
    size_t r; int n0 = tc_iph_count; errno = 0;
    r = fwrite("abcd", 1, 4, nul);
    CHECK_EQ_U(r, 0);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - n0, 1);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(null_stream);
    return tc_end();
}

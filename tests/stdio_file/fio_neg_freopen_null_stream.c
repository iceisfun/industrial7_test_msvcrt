/* TC
 * id:    fio_neg_freopen_null_stream
 * area:  stdio_file
 * kind:  negative
 * funcs: freopen
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs freopen (parameter validation: invalid stream -> EINVAL)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(null_stream)
{
    FILE *volatile s = NULL; FILE *volatile r; tc_iph_count = 0; errno = 0;
    r = freopen("tc_fio_neg_freopen_x.tmp", "r", s);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_stream); return tc_end(); }

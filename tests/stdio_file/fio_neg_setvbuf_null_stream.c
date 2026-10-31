/* TC
 * id:    fio_neg_setvbuf_null_stream
 * area:  stdio_file
 * funcs: setvbuf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs setvbuf; parameter validation
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>

static char b[1024];

TC_CASE(null)
{
    FILE *volatile p = NULL; volatile int r;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = setvbuf(p, b, _IOFBF, sizeof b);
    CHECK(r != 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(null); return tc_end(); }

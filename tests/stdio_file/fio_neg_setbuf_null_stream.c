/* TC
 * id:    fio_neg_setbuf_null_stream
 * area:  stdio_file
 * funcs: setbuf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs setbuf; parameter validation
 * note:  setbuf validates through the setvbuf path; MS docs do not explicitly document setbuf(NULL,..); EINVAL + one invalid-parameter call is inferred from setvbuf's documented validation.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>

static char b[BUFSIZ];

TC_CASE(null)
{
    FILE *volatile p = NULL;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    setbuf(p, b);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(null); return tc_end(); }

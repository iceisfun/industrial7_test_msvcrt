/* TC
 * id:    fio_neg_fileno_null
 * area:  stdio_file
 * funcs: _fileno
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _fileno (returns -1 and EINVAL for NULL)
 * note:  
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

static FILE * volatile nul = NULL;

TC_CASE(null_stream)
{
    int r; errno = 0; tc_iph_count = 0;
    r = _fileno(nul);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(null_stream);
    return tc_end();
}
